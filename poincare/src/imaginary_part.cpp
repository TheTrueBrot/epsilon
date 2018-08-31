#include <poincare/imaginary_part.h>
#include <poincare/layout_helper.h>
#include <poincare/serialization_helper.h>
#include <poincare/simplification_helper.h>
#include <poincare/rational.h>
#include <cmath>

namespace Poincare {

ImaginaryPartNode * ImaginaryPartNode::FailedAllocationStaticNode() {
  static AllocationFailureExpressionNode<ImaginaryPartNode> failure;
  TreePool::sharedPool()->registerStaticNodeIfRequired(&failure);
  return &failure;
}

LayoutReference ImaginaryPartNode::createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  return LayoutHelper::Prefix(ImaginaryPart(this), floatDisplayMode, numberOfSignificantDigits, name());
}

int ImaginaryPartNode::serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  return SerializationHelper::Prefix(this, buffer, bufferSize, floatDisplayMode, numberOfSignificantDigits, name());
}

template<typename T>
Complex<T> ImaginaryPartNode::computeOnComplex(const std::complex<T> c, Preferences::AngleUnit angleUnit) {
  return Complex<T>(std::imag(c));
}

Expression ImaginaryPartNode::shallowReduce(Context & context, Preferences::AngleUnit angleUnit) {
  return ImaginaryPart(this).shallowReduce(context, angleUnit);
}

Expression ImaginaryPart::shallowReduce(Context & context, Preferences::AngleUnit angleUnit) {
  Expression e = Expression::defaultShallowReduce(context, angleUnit);
  if (e.isUndefinedOrAllocationFailure()) {
    return e;
  }
  Expression c = childAtIndex(0);
#if MATRIX_EXACT_REDUCING
  if (c.type() == ExpressionNode::Type::Matrix) {
    return SimplificationHelper::Map(*this, context, angleUnit);
  }
#endif
  if (c.type() == ExpressionNode::Type::Rational) {
    return Rational(0);
  }
  return *this;
}

}


