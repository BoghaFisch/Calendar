#include "gregorian.hpp"

namespace lab2 {
	// Constructors
	Gregorian::Gregorian()
	{
		// If not using visual studio - use kattistime
		#if VISUAL_STUDIO == 0
			// För att få nuvarande tid
			time_t mytime;
			k_time(&mytime);

			// För att få ut datum lokalt 
			struct tm *t = gmtime(&mytime);
			d_year = t->tm_year + 1900;
			d_month = t->tm_mon + 1;      // månaderna och dagarna
			d_day = t->tm_mday;         // indexerade från ETT
			update_jdn();
			if (!is_valid_date())
				throw std::out_of_range("Error: Not a valid date");
		// Om vi använder visual studio - använd localtime
		#elif VISUAL_STUDIO == 1
			time_t rawtime;
			struct tm timeinfo;
			time(&rawtime);
			localtime_s(&timeinfo, &rawtime);
			d_year = timeinfo.tm_year + 1900;
			d_month = timeinfo.tm_mon + 1;
			d_day = timeinfo.tm_mday;
			update_jdn();
			if (!is_valid_date())
				throw std::out_of_range("Error: Not a valid date");
		#endif
	}
	// Konstruera från jdn
	Gregorian::Gregorian(int julian_date_number) {
		jdn = julian_date_number;
		refresh();
	}
	// Konstruera från year, month , day
	Gregorian::Gregorian(int y, unsigned m, unsigned d) {
		jdn = calculate_jdn(y, m, d);
		d_year = y;
		d_month = m;
		d_day = d;
		if (!is_valid_date())
			throw std::invalid_argument("Error: Invalid argument.");
	}
	// Copy constructors
	Gregorian::Gregorian(const Gregorian &other_gregorian) {
		copy(other_gregorian);
		if (!is_valid_date())
			throw std::out_of_range("Error: Not a valid date");
	}
	Gregorian::Gregorian(const Date &other_date) {
		jdn = other_date.get_jdn();
		refresh();
	}
	Gregorian::Gregorian(const Date * dp) {
		jdn = dp->get_jdn();
		refresh();
	}
	// Desctructor
	Gregorian::~Gregorian() { }

	// Copy assignment
	Gregorian & Gregorian::operator=(const Gregorian &other_gregorian) {
		copy(other_gregorian);
		if (!is_valid_date())
			throw std::out_of_range("Error: Not a valid date");
		return *this;
	}
	Gregorian & Gregorian::operator=(const Date &other_date) {
		jdn = other_date.get_jdn();
		refresh();
		return *this;
	}
	// Overloaded operators
	Gregorian Gregorian::operator++(int) {
		Gregorian tmp = *this;
		jdn++;
		refresh();
		return tmp;
	}
	Gregorian Gregorian::operator--(int) {
		Gregorian tmp = *this;
		jdn--;
		refresh();
		return tmp;
	}
	// Calculate the julian day number using the formula from wikipedia
	int Gregorian::calculate_jdn(int t_year, unsigned t_month, unsigned t_day) const {
		int a = (int)floor((14 - t_month) / 12);
		int y = t_year + 4800 - a;
		int m = t_month + 12 * a - 3;

		return (t_day + (int) floor((153.0 * m + 2.0) / 5.0) + 365 * y + (int)floor(y / 4.0) - (int)floor(y / 100.0) + (int)floor(y / 400.0) - 32045);
	}
	// Update jdn from current yymmdd
	void Gregorian::update_jdn() {
		jdn = calculate_jdn(d_year, d_month, d_day);
	}
	void Gregorian::refresh() {
		/*For Gregorian calendar :
			y 	4716 	v 	3
			j 	1401 	u 	5
			m 	2 	s 	153
			n 	12 	w 	2
			r 	4 	B 	274277
			p 	1461 	C 	-38

			1. f = J + j + (((4 × J + B) div 146097) × 3) div 4 + C
			2. e = r × f + v
			3. g = mod(e, p) div r
			4. h = u × g + w
			5. D = (mod(h, s)) div u + 1
			6. M = mod(h div s + m, n) + 1
			7. Y = (e div p) - y + (n + m - M) div n
		*/
		int f = jdn + 1401 + (int) floor(  ( (int) floor((4.0*jdn + 274277.0) / 146097.0) * 3) / 4.0) - 38;
		int e = 4 * f + 3;
		int g = (int)floor((e % 1461) / 4.0);
		int h = 5 * g + 2;
		d_day = (int)floor((h % 153) / 5.0) + 1;
		d_month = (((int)floor(h / 153) + 2) % 12) + 1;
		d_year = (int)floor(e / 1461) - 4716 + (int)floor((14 - d_month) / 12);

		// If the year we calculated is not allowed, throw exception
		if (!is_valid_date()) {
			throw std::out_of_range("Error: Invalid date");
		}
	}
	// http://www.timeanddate.com/date/leapyear.html
	bool Gregorian::is_leap_year() const {
		if (d_year % 4 == 0) {
			if (d_year % 100 == 0) {
				if (d_year % 400 == 0)
					return true;
				else {
					return false;
				}
			}
		}
		else return false;
	}
}
