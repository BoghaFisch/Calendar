#ifndef GREGORIAN_HPP
#define GREGORIAN_HPP

#include "date.hpp"

namespace lab2 {
	class Gregorian : public Date
	{
	private:
		void update_jdn() override;
		int calculate_jdn(int, unsigned, unsigned) const override;
		void refresh() override;
		bool is_leap_year() const override;


	protected:
		
	public:
		// Constructors
		Gregorian();
		Gregorian(int);
		Gregorian(int, unsigned, unsigned);
		Gregorian(const Gregorian &);
		Gregorian(const Date &);
		Gregorian(const Date *);

		// Destructor
		~Gregorian();

		// Copy assignment
		Gregorian & operator=(const Gregorian &);
		Gregorian & operator=(const Date &);

		// Overloaded operators
		using Date::operator++;
		using Date::operator--;
		Gregorian operator++(int);
		Gregorian operator--(int);
	};
}
#endif

