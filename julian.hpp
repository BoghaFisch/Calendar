#ifndef JULIAN_HPP
#define JULIAN_HPP

#include "date.hpp"

namespace lab2 {
	class Julian : public Date
	{
	private:
		void update_jdn() override;
		int calculate_jdn(int, unsigned, unsigned) const override;
		void refresh() override;
		bool is_leap_year() const override;

	protected:


	public:
		// Constructors
		Julian();
		Julian(int);
		Julian(int, unsigned, unsigned);
		Julian(const Julian &);
		Julian(const Date &);
		Julian(const Date *);

		// Desctructor
		~Julian();

		// Copy assignment
		Julian & operator=(const Julian &);
		Julian & operator=(const Date &);

		// Overloaded operators
		using Date::operator++;
		using Date::operator--;
		Julian operator++(int);
		Julian operator--(int);
	};
}
#endif

