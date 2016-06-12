#ifndef HUM_LOG_HPP
#define HUM_LOG_HPP
#include <string>
#include <sstream>
#include <iostream>
#include "hum.hpp"

namespace hum
{
/*!
  \brief Check a condition and if it fails, exit the program and print the
  message.

  This method does nothing if NDEBUG is defined.
*/
inline void assert_msg(bool condition, const char* message)
{
#ifndef NDEBUG
    if (!condition)
    {
        std::cerr << "Assertion failed in " << __FILE__
                    << " line " << __LINE__ << ": " << message << std::endl;
        std::exit(EXIT_FAILURE);
    }
#endif
}

/*!
  \brief Check a condition and if it fails, exit the program and print the
  message.

  This method does nothing if NDEBUG is defined.
*/
inline void assert_msg(bool condition, const std::string& message)
{
#ifndef NDEBUG
    if (!condition)
    {
        std::cerr << "Assertion failed in " << __FILE__
                    << " line " << __LINE__ << ": " << message << std::endl;
        std::exit(EXIT_FAILURE);
    }
#endif
}

namespace detail
{
template <typename T>
inline void log(std::ostream& out, const T& message)
{
    out << message;
}

inline void log(std::ostream& out, const char* message)
{
    out << message;
}

template <typename T>
inline void log(std::ostream& out, const hum::Vector2<T>& vector)
{
    out << "hum::Vector2( " << vector.x << ", "
                      << vector.y
        << " )";
}

template <typename T>
inline void log(std::ostream& out, const hum::Vector3<T>& vector)
{
    out << "hum::Vector3( " << vector.x << ", "
                      << vector.y << ", "
                      << vector.z
        << " )";
}

inline void log(std::ostream& out, const hum::Transformation& transform)
{
    out << "hum::Transformation ( "
        << "position=";
    log(out, transform.position);
    out << "; rotation=";
    log(out, transform.rotation);
    out << "; scale=";
    log(out, transform.scale);
    out << " )";
}

inline void log(std::ostream& out, const hum::Time& time)
{
    out << "hum::Time ( " << time.asMilliseconds() << " ms )";
}

inline void log(std::ostream& out, const hum::Clock& clock)
{
    out << "hum::Clock ( " << clock.getTime().asMilliseconds() << " ms )";
}
} /* detail */

/*!
  \brief Print a message to the standard output.
*/
inline void log(const char* message)
{
    detail::log(std::cout, message);
    std::cout << std::endl;
}

/*!
  \brief Print a message to the standard output.

  T can be any type that has the operator << overloaded.
  It can also be any of the following classes:
  \li hum::Vector2
  \li hum::Vector3
  \li hum::Transformation
  \li hum::Time
  \li hum::Clock
*/
template <typename T>
inline void log(const T& message)
{
    detail::log(std::cout, message);
    std::cout << std::endl;
}

/*!
  \brief Print a message to the error output.
*/
inline void log_e(const char* message)
{
    detail::log(std::cerr, message);
    std::cerr << std::endl;
}

/*!
  \brief Print a message to the error output.

  T can be any type that has the operator << overloaded.
  It can also be any of the following classes:
  \li hum::Vector2
  \li hum::Vector3
  \li hum::Transformation
  \li hum::Time
  \li hum::Clock
*/
template <typename T>
inline void log_e(const T& message)
{
    detail::log(std::cerr, message);
    std::cerr << std::endl;
}

/*!
  \brief Print a message to the standard output.

  This method does nothing if NDEBUG is defined.
*/
inline void log_d(const char* message)
{
#ifndef NDEBUG
    log(message);
#endif
}

/*!
  \brief Print a message to the standard output.

  This method does nothing if NDEBUG is defined.

  T can be any type that has the operator << overloaded.
  It can also be any of the following classes:
  \li hum::Vector2
  \li hum::Vector3
  \li hum::Transformation
  \li hum::Time
  \li hum::Clock
*/
template <typename T>
inline void log_d(const T& message)
{
#ifndef NDEBUG
    log(message);
#endif
}
}
#endif
