#ifndef STAN__MATH__ERROR_HANDLING__MATRIX__CHECK_POS_SEMIDEFINITE_HPP
#define STAN__MATH__ERROR_HANDLING__MATRIX__CHECK_POS_SEMIDEFINITE_HPP

#include <sstream>
#include <stan/math/matrix/Eigen.hpp>
#include <stan/math/error_handling/dom_err.hpp>
#include <stan/math/error_handling/matrix/constraint_tolerance.hpp>
#include <boost/math/special_functions/fpclassify.hpp>

namespace stan {
  namespace math {

    /**
     * Return <code>true</code> if the specified matrix is positive definite
     *
     * NOTE: symmetry is NOT checked by this function
     * 
     * @param function
     * @param y Matrix to test.
     * @param name
     * @param result
     * @return <code>true</code> if the matrix is positive semi-definite.
     * @return throws if any element in y is nan
     * @tparam T Type of scalar.
     */
    // FIXME: update warnings (message has (0,0) item)
    template <typename T_y, typename T_result>
    inline bool check_pos_semidefinite(const char* function,
                                       const Eigen::Matrix<T_y,Eigen::Dynamic,Eigen::Dynamic>& y,
                                       const char* name,
                                       T_result* result) {
      typedef 
        typename Eigen::Matrix<T_y,Eigen::Dynamic,Eigen::Dynamic>::size_type 
        size_type;
      if (y.rows() == 1 && !(y(0,0) >= 0.0)) {
        std::ostringstream message;
        message << name << " is not positive semi-definite. " 
                << name << "(0,0) is %1%.";
        std::string msg(message.str());
        return dom_err(function,y(0,0),name,msg.c_str(),"",result);
      }
      Eigen::LDLT< Eigen::Matrix<T_y,Eigen::Dynamic,Eigen::Dynamic> > cholesky 
        = y.ldlt();
      if(cholesky.info() != Eigen::Success || (cholesky.vectorD().array() < 0.0).any()) {
        std::ostringstream message;
        message << name << " is not positive semi-definite. " 
                << name << "(0,0) is %1%.";
        std::string msg(message.str());
        return dom_err(function,y(0,0),name,msg.c_str(),"",result);
      }
      for (int i = 0; i < y.size(); i++)
        if (boost::math::isnan(y(i))) {
          std::ostringstream message;
          message << name << " is not positive semi-definite. " 
                  << name << "(0,0) is %1%.";
          std::string msg(message.str());
          return dom_err(function,y(0,0),name,msg.c_str(),"",result);
        }
      return true;
    }

  }
}
#endif
