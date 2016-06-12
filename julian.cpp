#include "julian.hpp"

namespace lab2 {
	Julian::Julian() {
		// Om vi använder visual studio - använd ej kattistime
		#if VISUAL_STUDIO == 1
			time_t rawtime;
			struct tm timeinfo;
			time(&rawtime);
			localtime_s(&timeinfo, &rawtime);
			int t_year = timeinfo.tm_year + 1900;
			unsigned t_month = timeinfo.tm_mon + 1;
			unsigned t_day = timeinfo.tm_mday;	
		// Om vi inte använder visual studio - använd kattistime
		#elif VISUAL_STUDIO == 0
			// För att få nuvarande tid
			time_t mytime;
			k_time(&mytime);

			// För att få ut datum lokalt 
			struct tm *t = gmtime(&mytime);
			int t_year  = t->tm_year + 1900;
			unsigned t_month = t->tm_mon + 1;      // månaderna och dagarna
			unsigned t_day   = t->tm_mday;         // indexerade från ETT
		#endif
		// Uppdatera julian date number
		int a = (int)floor((14 - t_month) / 12);
		int y = t_year + 4800 - a;
		int m = t_month + 12 * a - 3;
		jdn = t_day + (int)floor((153.0 * m + 2.0) / 5.0) + 365 * y + (int)floor(y / 4.0) - (int)floor(y / 100.0) + (int)floor(y / 400.0) - 32045;
		
		// Since the fetched date is not a julian date - refresh to calculate date from jdn instead.
		refresh();
	}
	Julian::Julian(int julian_date_number) {
		jdn = julian_date_number;
		refresh();
	}
	Julian::Julian(int y, unsigned m, unsigned d) {
		jdn = calculate_jdn(y, m, d);
		d_year = y;
		d_month = m;
		d_day = d;
		if (!is_valid_date()) 
			throw std::invalid_argument("Error: Invalid argument.");
	}
	// Copy constructors
	Julian::Julian(const Julian &other_julian) {
		copy(other_julian);
		if (!is_valid_date())
			throw std::out_of_range("Error: Not a valid date");
	}
	Julian::Julian(const Date &other_date) {
		jdn = other_date.get_jdn();
		refresh();
	}
	// Construct from datepointer
	Julian::Julian(const Date * dp) {
		jdn = dp->get_jdn();
		refresh();
	}
	// Destructor 
	Julian::~Julian() { }

	// Copy assignment operator
	Julian & Julian::operator=(const Julian & other_julian) {
		copy(other_julian);
		if (!is_valid_date())
			throw std::out_of_range("Error: Not a valid date");
		return *this;
	}
	Julian & Julian::operator=(const Date & other_date) {
		jdn = other_date.get_jdn();
		refresh();
		return *this;
	}

	// Other operators
	Julian Julian::operator++(int) {
		Julian tmp = *this;
		jdn++;
		refresh();
		return tmp;
	}
	Julian Julian::operator--(int) {
		Julian tmp = *this;
		jdn--;
		refresh();
		return tmp;
	}
	// Räknar ut julian date number med formeln från wikipedia
	int Julian::calculate_jdn(int t_year, unsigned t_month, unsigned t_day) const {
		int a = (int) floor((14 - t_month) / 12);
		int y = t_year + 4800 - a;
		int m = t_month + 12 * a - 3;
		return (t_day + ((int)floor((153 * m + 2) / 5)) + 365 * y + ((int)floor(y / 4)) - 32083);
	}
	void Julian::update_jdn()
	{
		jdn = calculate_jdn(d_year, d_month, d_day);
	}
	// Uppdaterar year, month och day baserat baserat på jdn
	void Julian::refresh() {
		/*	y 	4716 	v 	3
			j 	1401 	u 	5
			m 	2 	s 	153
			n 	12 	w 	2
			r 	4 	B 	274277
			p 	1461 	C 	-38

			f = J + j
			2. e = r × f + v
			3. g = mod(e, p) div r
			4. h = u × g + w
			5. D = (mod(h, s)) div u + 1
			6. M = mod(h div s + m, n) + 1
			7. Y = (e div p) - y + (n + m - M) div n
		*/
		int f = jdn + 1401;
		unsigned long e = 4 * f + 3;
		int g = (int)floor((e % 1461) / 4.0);
		unsigned long h = 5 * g + 2;
		d_day = (int) floor((h % 153) / 5) + 1;
		d_month = (((int) floor(h / 153) + 2) % 12) + 1;
		d_year = (int)floor(e / 1461) - 4716 + (int)floor((14 - d_month) / 12);

		// Om året vi räknat ut ej är tilllåtet - Kasta exception
		if (!is_valid_date())
			throw std::out_of_range("Error: invalid date");
	}
	bool Julian::is_leap_year() const {
		return (d_year % 4) == 0;
	}
}
