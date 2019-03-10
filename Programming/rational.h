/*****************************************************************************/
/* Name: rational.h                                                          */
/* Uses: Class for basic rational number arithmetic.                         */
/* Date: 4/19/2011                                                           */
/* Author: Andrew Que <http://www.DrQue.net/>                                */
/* Revisions:                                                                */
/*   2011/04/19 - QUE - Creation.                                            */
/*                                                                           */
/* License:                                                                  */
/*   This program is free software: you can redistribute it and/or modify    */
/*   it under the terms of the GNU General Public License as published by    */
/*   the Free Software Foundation, either version 3 of the License, or       */
/*   (at your option) any later version.                                     */
/*                                                                           */
/*   This program is distributed in the hope that it will be useful,         */
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           */
/*   GNU General Public License for more details.                            */
/*                                                                           */
/*   You should have received a copy of the GNU General Public License       */
/*   along with this program.  If not, see <http://www.gnu.org/licenses/>.   */
/*                                                                           */
/*                     (C) Copyright 2011 by Andrew Que                      */
/*                           http://www.DrQue.net/                           */
/*****************************************************************************/
#ifndef _RATIONAL_H_
#define _RATIONAL_H_

#include <cfloat>   // For 'DBL_MAX'.
#include <cmath>    // For 'abs' on floating point.
#include <cstdlib>  // For 'abs' on integer.
#include <ostream>  // For 'std::ostream'.
#include <limits>

//=============================================================================
// Rational number class.
//
// Background:
//    Rational numbers have the form of a/b, where 'a' is the numerator and
//  'b' is the denominator.  Using operations such as multiplication and
//  division by integer or rational numbers always results in an other rational
//  number.  In some cases, rational numbers may be more exact then floating
//  point values.  For example, 1/3 is approx 0.3333, but not exactly.  This
//  class will allow for the exact representation of rational values, which may
//  be useful for lengthy mathematical processes where error can add up, or
//  when an exact rational result is desired.
//
// Implementation:
//    This class is a template and requires a base type.  In most cases, this
//  will be 'int'.  However, unsigned and longs may also be useful depending
//  on the range and scale of the numbers worked with.
//    All basic arithmetic operators have been implemented (addition,
//  subtraction, multiplication, and division).  The assign operation, equal,
//  and not-equal logic operators are available.  Therefore this class may be
//  syntactically used like any number type.
//
// Notes:
//    This class has no overflow check, and is limited by the template type.
//  Problems can occur when adding or subtracting, as the values must be
//  multiplied up before adding can be done.
//=============================================================================
template< class TYPE = int, class FLOAT_TYPE = double >
  class Rational
  {
    protected:
      TYPE numerator;
      TYPE denominator;

      //-----------------------------------------------------------------------
      // Use:
      //   Put the rational into a standard form.  Reduce as much as possible.
      //
      // Standards:
      //   Zero is of the form 0/1.
      //   Divide-by-zero error will result in 0/0.
      //   All whole numbers will be in the form n/1.
      //   a/b will always be reduced.
      //-----------------------------------------------------------------------
      void reduce()
      {
        // Divide-by-zero?
        if ( 0 == denominator )
        {
          numerator = 0;
        }
        else
        // Zero?
        if ( 0 == numerator )
          denominator = 1;
        else
        // Whole number?
        if ( 0 == ( numerator % denominator ) )
        {
          numerator   /= denominator;
          denominator  = 1;
        }
        else
        {
          // Find Greatest Common Divisor (GCD) to reduce.
          TYPE valueA = numerator;
          TYPE valueB = denominator;
          while ( valueA != 0 )
          {
            TYPE hold = valueA;
            valueA = valueB % valueA;
            valueB = hold;
          }

          // Reduce by GCD (thus, making the new GCF=1).
          numerator   /= valueB;
          denominator /= valueB;
        }

        // Keep the sign on the numerator.
        if ( denominator < 0 )
        {
          numerator   = -numerator;
          denominator = -denominator;
        }

      } // reduce

      //-----------------------------------------------------------------------
      // Return the largest value that can be hold by the type.
      //-----------------------------------------------------------------------
      TYPE typeMax() const
      {
        TYPE value;
        TYPE lastValue;

        lastValue = 0;
        value     = 0xFF; // <- Type is at least 8-bit.
        while ( ( lastValue != value )
             && ( value > 0 ) )
        {
          lastValue = value;
          value |= ( value << 1 );
        }

        if ( value < 0 )
          value = lastValue;

        return value;
      }

    public:

      //=======================================================================
      // Set functions.
      //=======================================================================

      //-----------------------------------------------------------------------
      // Set the numerator.
      // NOTE: Do not call this followed by setDenominator--fraction is
      // reduced.
      //-----------------------------------------------------------------------
      void setNumerator
      (
        TYPE const & value
      )
      {
        numerator = value;
        reduce();
      }

      //-----------------------------------------------------------------------
      // Set the denominator.
      // NOTE: Do not call this followed by setNumerator--fraction is
      // reduced.  Example:
      //    Rational< int > value( 1, 3 );
      //    value.setNumerator( 3 )    // <- Rational will be 1/1
      //    value.setDenominator( 8 ); // <- Rational will not be 1/8, not 3/8
      //-----------------------------------------------------------------------
      void setDenominator
      (
        TYPE const & value
      )
      {
        denominator = value;
        reduce();
      }

      //-----------------------------------------------------------------------
      // General assignment operator.
      //-----------------------------------------------------------------------
      template< class INPUT_TYPE >
        Rational const & operator =
        (
          INPUT_TYPE const & value
        )
        {
          // An integer type?
          if ( std::numeric_limits< INPUT_TYPE >::is_integer )
          {
            // Try and use the integer type.
            *this = static_cast< TYPE >( value );
          }
          else
          // Some kind of floating point?
          if ( std::numeric_limits< INPUT_TYPE >::is_specialized )
          {
            // Try and cast to a floating point.
            *this = static_cast< FLOAT_TYPE >( value );
          }
          else
            // Must be a class.  Try and convert.
            *this = value;

          return *this;
        }

      //-----------------------------------------------------------------------
      // Set rational to whole integer-type number.
      //-----------------------------------------------------------------------
      Rational const & operator =
      (
        TYPE const & value
      )
      {
        numerator   = value;
        denominator = 1;

        return *this;
      }

      //-----------------------------------------------------------------------
      // Set rational to an other rational.
      //-----------------------------------------------------------------------
      Rational const & operator =
      (
        Rational const & value
      )
      {
        numerator   = value.numerator;
        denominator = value.denominator;
        reduce();

        return *this;
      }

      //-----------------------------------------------------------------------
      // Uses:
      //   Change any real number into a rational number.  Often an
      // approximation.
      //
      // Credits:
      //   David Eppstein / UC Irvine / 8 Aug 1993
      //   Arno Formella, May 2008
      //   Andrew Que (this implementation)
      //
      // Limits:
      //   This function is really only good for small numbers.
      // Approximations of irrational numbers are not all that useful.
      //
      //-----------------------------------------------------------------------
      Rational const & operator =
      (
        // Floating point value to convert to rational.
        FLOAT_TYPE value
      )
      {
        TYPE matrix[ 2 ][ 2 ];

        // Set to NaN.
        numerator   = 0;
        denominator = 0;

        // Initialize matrix.
        matrix[ 0 ][ 0 ] = 1;
        matrix[ 0 ][ 1 ] = 0;
        matrix[ 1 ][ 0 ] = 0;
        matrix[ 1 ][ 1 ] = 1;

        FLOAT_TYPE initialValue = value;
        FLOAT_TYPE lastError = DBL_MAX;
        FLOAT_TYPE error;
        TYPE accumulator;

        // Loop until number is either exactly represented, or we run out of precision.
        do
        {
          TYPE hold;

          // Get the integer part of 'value'.
          accumulator = static_cast< TYPE >( value );

          // Compute new numerator.
          hold = ( matrix[ 0 ][ 0 ] * accumulator ) + matrix[ 0 ][ 1 ];
          matrix[ 0 ][ 1 ] = matrix[ 0 ][ 0 ];
          matrix[ 0 ][ 0 ] = hold;

          // Compute new denominator.
          hold = ( matrix[ 1 ][ 0 ] * accumulator ) + matrix[ 1 ][ 1 ];
          matrix[ 1 ][ 1 ] = matrix[ 1 ][ 0 ];
          matrix[ 1 ][ 0 ] = hold;

          // Check for divide by zero condition.
          if ( value != accumulator )
            value = 1.0 / ( value - accumulator );

          // See how much error exists between our guess and the actual
          // value.
          error  = matrix[ 0 ][ 0 ];
          error /= matrix[ 1 ][ 0 ];
          error  = initialValue - error;
          error  = fabs( error );

          // If the error is better then the last error, use this value.
          if ( error < lastError )
          {
            lastError = error;
            numerator   = matrix[ 0 ][ 0 ];
            denominator = matrix[ 1 ][ 0 ];
          }

        }
        while ( ( value != accumulator )
             && ( error <= lastError ) );

        return *this;

      } // operator=

      //=======================================================================
      // Multiplication.
      //=======================================================================

      //-----------------------------------------------------------------------
      // Multiplication by integer-type.
      //-----------------------------------------------------------------------
      Rational const operator *
      (
        TYPE const & value
      ) const
      {
        return Rational
          (
            numerator * value,
            denominator
          );
      }

      //-----------------------------------------------------------------------
      // Multiplication by rational.
      //-----------------------------------------------------------------------
      Rational const operator *
      (
        Rational const & value
      ) const
      {
        return Rational
          (
            numerator   * value.numerator,
            denominator * value.denominator
          );
      }

      //-----------------------------------------------------------------------
      // Multiply self by rational.
      //-----------------------------------------------------------------------
      Rational const & operator *=
      (
        Rational const & value
      )
      {
        *this = *this * value;

        return *this;
      }

      //-----------------------------------------------------------------------
      // Multiply self by integer-type.
      //-----------------------------------------------------------------------
      Rational const & operator *=
      (
        TYPE const & value
      )
      {
        *this = *this * value;

        return *this;
      }

      //=======================================================================
      // Division.
      //=======================================================================

      //-----------------------------------------------------------------------
      // Divide by integer-type.
      //-----------------------------------------------------------------------
      Rational const operator /
      (
        TYPE const & value
      ) const
      {
        return Rational
          (
            numerator,
            denominator * value
          );
      }

      //-----------------------------------------------------------------------
      // Divide by rational.
      //-----------------------------------------------------------------------
      Rational const operator /
      (
        Rational const & value
      ) const
      {
        return Rational
          (
            numerator   * value.denominator,
            denominator * value.numerator
          );
      }

      //-----------------------------------------------------------------------
      // Divide self by rational.
      //-----------------------------------------------------------------------
      Rational const & operator /=
      (
        Rational const & value
      )
      {
        *this = *this / value;

        return *this;
      }

      //-----------------------------------------------------------------------
      // Divide self by integer-type.
      //-----------------------------------------------------------------------
      Rational const & operator /=
      (
        TYPE const & value
      )
      {
        *this = *this / value;

        return *this;
      }

      //=======================================================================
      // Addition.
      //=======================================================================

      //-----------------------------------------------------------------------
      // Add by rational.
      //-----------------------------------------------------------------------
      Rational const operator +
      (
        Rational const & value
      ) const
      {
        TYPE newNumerator   = numerator;
        TYPE newDenominator = denominator;
        TYPE otherNumerator = value.numerator;

        if ( denominator != value.denominator )
        {
          newNumerator   *= value.denominator;
          newDenominator *= value.denominator;
          otherNumerator *= denominator;
        }

        newNumerator += otherNumerator;

        return Rational( newNumerator, newDenominator );
      }

      //-----------------------------------------------------------------------
      // Add by integer-type.
      //-----------------------------------------------------------------------
      Rational const operator +
      (
        TYPE const & value
      ) const
      {
        return Rational( numerator + (value * denominator) , denominator );
      }

      //-----------------------------------------------------------------------
      // Add self by rational.
      //-----------------------------------------------------------------------
      Rational const & operator +=
      (
        Rational const & value
      )
      {
        *this = *this + value;

        return *this;
      }

      //-----------------------------------------------------------------------
      // Add self by integer-type.
      //-----------------------------------------------------------------------
      Rational const & operator +=
      (
        TYPE const & value
      )
      {
        *this = *this + value;

        return *this;
      }

      //=======================================================================
      // Subtractions.
      //=======================================================================

      //-----------------------------------------------------------------------
      // Subtract by rational.
      //-----------------------------------------------------------------------
      Rational const operator -
      (
        Rational const & value
      ) const
      {
        TYPE newNumerator   = numerator;
        TYPE newDenominator = denominator;
        TYPE otherNumerator = value.numerator;

        if ( denominator != value.denominator )
        {
          newNumerator   *= value.denominator;
          newDenominator *= value.denominator;
          otherNumerator *= denominator;
        }

        newNumerator -= otherNumerator;

        return Rational( newNumerator, newDenominator );
      }

      //-----------------------------------------------------------------------
      // Subtract by integer-type.
      //-----------------------------------------------------------------------
      Rational const operator -
      (
        TYPE const & value
      ) const
      {
        return
          Rational
          (
            numerator - (value * denominator) ,
            denominator
          );
      }

      //-----------------------------------------------------------------------
      // Subtract self by rational.
      //-----------------------------------------------------------------------
      Rational const & operator -=
      (
        Rational const & value
      )
      {
        *this = *this - value;

        return *this;
      }

      //-----------------------------------------------------------------------
      // Subtract self by integer-type.
      //-----------------------------------------------------------------------
      Rational const & operator -=
      (
        TYPE const & value
      )
      {
        *this = *this - value;

        return *this;
      }

      //=======================================================================
      // Logic operators.
      //=======================================================================

      //-----------------------------------------------------------------------
      // Equality operator test between self and rational.
      //-----------------------------------------------------------------------
      bool operator ==
      (
        Rational const & value
      ) const
      {
        return ( ( numerator == value.numerator )
              && ( denominator == value.denominator ) );
      }

      //-----------------------------------------------------------------------
      // Not equal operator test between self and rational.
      //-----------------------------------------------------------------------
      bool operator !=
      (
        Rational const & value
      ) const
      {
        return !( *this == value );
      }

      //-----------------------------------------------------------------------
      // Equality operator test between self and integer-type.
      //-----------------------------------------------------------------------
      bool operator ==
      (
        TYPE const & value
      ) const
      {
        return ( ( numerator == value )
              && ( denominator == 1 ) );
      }

      //-----------------------------------------------------------------------
      // Not equal operator test between self and integer-type.
      //-----------------------------------------------------------------------
      bool operator !=
      (
        int const value
      ) const
      {
        return !( *this == value );
      }

      //-----------------------------------------------------------------------
      // Greater than operator.
      //-----------------------------------------------------------------------
      bool operator >
      (
        TYPE const & value
      ) const
      {
        return ( numerator > ( value * denominator ) );
      }

      //-----------------------------------------------------------------------
      // Less than operator.
      //-----------------------------------------------------------------------
      bool operator <
      (
        TYPE const & value
      ) const
      {
        return ( numerator < ( value * denominator ) );
      }

      //-----------------------------------------------------------------------
      // Greater than operator.
      //-----------------------------------------------------------------------
      bool operator >
      (
        Rational const & value
      ) const
      {
        TYPE thisNumerator   = numerator;
        TYPE otherNumerator  = value.numerator;

        // If there isn't a common denominator...
        if ( denominator != value.denominator )
        {
          // Create common denominator, and adjust numerators
          thisNumerator  *= value.denominator;
          otherNumerator *= denominator;
        }

        return ( thisNumerator > otherNumerator );
      }

      //-----------------------------------------------------------------------
      // Less than rational operator.
      //-----------------------------------------------------------------------
      bool operator <
      (
        Rational const & value
      ) const
      {
        TYPE thisNumerator   = numerator;
        TYPE otherNumerator  = value.numerator;

        // If there isn't a common denominator...
        if ( denominator != value.denominator )
        {
          // Create common denominator, and adjust numerators
          thisNumerator  *= value.denominator;
          otherNumerator *= denominator;
        }

        return ( thisNumerator < otherNumerator );
      }

      //-----------------------------------------------------------------------
      // Greater or equal-to operator.
      //-----------------------------------------------------------------------
      bool operator >=
      (
        TYPE const & value
      ) const
      {
        return ( numerator >= ( value * denominator ) );
      }

      //-----------------------------------------------------------------------
      // Less or equal-to operator.
      //-----------------------------------------------------------------------
      bool operator <=
      (
        TYPE const & value
      ) const
      {
        return ( numerator <= ( value * denominator ) );
      }

      //-----------------------------------------------------------------------
      // Greater pr equal-to rational operator.
      //-----------------------------------------------------------------------
      bool operator >=
      (
        Rational const & value
      ) const
      {
        TYPE thisNumerator   = numerator;
        TYPE otherNumerator  = value.numerator;

        // If there isn't a common denominator...
        if ( denominator != value.denominator )
        {
          // Create common denominator, and adjust numerators
          thisNumerator  *= value.denominator;
          otherNumerator *= denominator;
        }

        return ( thisNumerator >= otherNumerator );
      }

      //-----------------------------------------------------------------------
      // Less or equal-to operator.
      //-----------------------------------------------------------------------
      bool operator <=
      (
        Rational const & value
      ) const
      {
        TYPE thisNumerator   = numerator;
        TYPE otherNumerator  = value.numerator;

        // If there isn't a common denominator...
        if ( denominator != value.denominator )
        {
          // Create common denominator, and adjust numerators
          thisNumerator  *= value.denominator;
          otherNumerator *= denominator;
        }

        return ( thisNumerator <= otherNumerator );
      }

      //=======================================================================
      // Constructors.
      //=======================================================================

      //-----------------------------------------------------------------------
      // Constructor from whole number.
      //-----------------------------------------------------------------------
     Rational
      (
        TYPE const & value
      )
      :
        numerator( value ),
        denominator( 1 )
      {
      }

      template< class INPUT_TYPE >
        Rational
        (
          INPUT_TYPE const & value
        )
        {
          if ( std::numeric_limits< INPUT_TYPE >::is_integer )
          {
            numerator   = static_cast< TYPE >( value );
            denominator = 1;
          }
          else
          if ( std::numeric_limits< INPUT_TYPE >::is_specialized )
          {
            *this = static_cast< FLOAT_TYPE >( value );
          }
          else
            *this = value;
        }

      //-----------------------------------------------------------------------
      // Constructor two integers, numerator and denominator.
      //-----------------------------------------------------------------------
      Rational
      (
        TYPE const & numeratorParameter,
        TYPE const & denominatorParameter
      )
      :
        numerator( numeratorParameter ),
        denominator( denominatorParameter )
      {
        reduce();
      }

      //-----------------------------------------------------------------------
      // Constructor from rational.
      //-----------------------------------------------------------------------
      Rational
      (
        Rational const & value
      )
      {
        *this = value;
      }

      //-----------------------------------------------------------------------
      // Empty constructor.
      //-----------------------------------------------------------------------
      Rational()
      :
        // Default value is 0/1, or 0.
        numerator( 0 ),
        denominator( 1 )
      {
      }

      //=======================================================================
      // Get functions.
      //=======================================================================

      //-----------------------------------------------------------------------
      // Return numerator.
      //-----------------------------------------------------------------------
      TYPE const & getNumerator() const
      {
        return numerator;
      }

      //-----------------------------------------------------------------------
      // Return denominator.
      //-----------------------------------------------------------------------
      TYPE const & getDenominator() const
      {
        return denominator;
      }

      //-----------------------------------------------------------------------
      // Return rational as a floating-point number.
      //-----------------------------------------------------------------------
      FLOAT_TYPE getAsDouble() const
      {
        FLOAT_TYPE result;

        result  = numerator;
        result /= denominator;

        return result;
      }
  };

//-----------------------------------------------------------------------------
// Negative operator for a rational number.
//-----------------------------------------------------------------------------
template< class TYPE, class FLOAT_TYPE >
  Rational< TYPE, FLOAT_TYPE > operator -
  (
    // Rational number to make negative.
    Rational< TYPE, FLOAT_TYPE > const & value
  )
  {
    return Rational< TYPE >( -value.getNumerator(), value.getDenominator() );
  }

//-----------------------------------------------------------------------------
// Stream output operator for rational number--turns rational into a string.
//-----------------------------------------------------------------------------
template< class TYPE, class FLOAT_TYPE >
  std::ostream & operator <<
  (
    // Stream in which to place string.
    std::ostream & stream,

    // Rational value to turn into string.
    Rational< TYPE, FLOAT_TYPE > const & value
  )
  {
    // Get numerator and denominator from parameter.
    TYPE numerator   = value.getNumerator();
    TYPE denominator = value.getDenominator();

    // Divide-by-zero error?
    if ( 0 == denominator )
      stream << "NaN";
    else
    // Whole number?
    if ( 1 == denominator )
      stream << numerator;
    else
    // Whole number with fractional part?
    if ( abs( numerator ) > denominator )
    {
      TYPE whole;
      TYPE remainder;

      whole     = numerator / denominator;
      remainder = numerator % denominator;

      if ( remainder < 0 )
         remainder = -remainder;

      stream << whole << " " << remainder << "/" << denominator;
    }
    else
      // Just a fraction.
      stream << numerator << "/" << denominator;

    return stream;
  }

#endif // _RATIONAL_H_
