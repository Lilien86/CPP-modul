/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lauger <lauger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:31:13 by marvin            #+#    #+#             */
/*   Updated: 2024/10/01 12:05:59 by lauger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_HPP
#define FIXED_HPP
#include <iostream>

class Fixed
{
	private:
		static const int _frac_bit_nb = 8;
		int _fp_value;
	public:
	  Fixed ();
	  Fixed (const Fixed &a);
	  ~Fixed ();
	  Fixed &operator=(const Fixed &a);
	  int   getRawBits(void) const;
	  void   setRawBits(int const raw);
	  float toFloat(void) const;
	  int toInt(void) const;
	
	
};

#endif