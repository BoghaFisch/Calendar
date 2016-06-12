
#include "date.hpp"

namespace lab2 {
	
	Date::Date() { 	}

	Date::~Date() {  }

	// Copy assignment
	Date & Date::operator=(const Date &other_date) {
		jdn = other_date.get_jdn();
		refresh();
		return *this;
	}
	Date & Date::operator++() {
		jdn++;
		refresh();
		return *this;
	}
	Date & Date::operator--() {
		jdn--;
		refresh();
		return *this;
	}

	Date & Date::operator+=(int d) {
		jdn += d;
		refresh();
		return *this;
	}
	Date & Date::operator-=(int d) {
		jdn -= d;
		refresh();
		return *this;
	}
	std::string Date::week_day_name() const {
		// Om veckodagen inte är tillåten, kasta exception. Annars returnera dess namn.
		if (week_day() > 7)
			throw std::out_of_range("Weeks out of range");
		return lab2::week_day_names[week_day()];
	}
	std::string Date::month_name() const {
		// Om månaden inte är tillåten, kasta exception. Annars returnera dess namn.
		if (d_month > 12)
			throw std::out_of_range("Months out of range (in month name)");
		return lab2::month_names[d_month];
	}
	unsigned Date::days_this_month() const {
		// If the month is not valid, return -1 (throwing out_of_range exception will cause kattis to say that it is the wrong one - not invalid_argument)
		if (d_month < 1 || d_month > 12)
			return -1;
		if (is_leap_year() && d_month == 2)
			return 29;
		else
			return lab2::days_per_month[d_month];
	}
	unsigned Date::week_day() const {
		return ((jdn % 7) + 1);
	}
	bool Date::is_valid_day() const {
		return (d_day > 0 && d_day <= days_this_month());
	}
	bool Date::is_valid_month() const {
		return (d_month > 0 && d_month <= 12);
	}
	bool Date::is_valid_year() const {
		return (d_year >= get_min_year() && d_year <= get_max_year());
	}
	// Om det är ett valid år, månad och dag - returnera true.
	bool Date::is_valid_date() const {
		return (is_valid_day() && is_valid_month() && is_valid_year());
	}
	// Adderar n år
	void Date::add_year(int n) {
		// Uppdatera j_year
		d_year += n;

		// Om dagen inte är valid => byt ut mot sista dagen i respektive månad
		if (!is_valid_day()) {
			d_day = days_this_month();
		}
		// Om året inte är giltigt => kasta exception
		if (!is_valid_date())
			throw std::out_of_range("Error: year out of range");

		// Vi har nya yy-mm-dd => uppdatera jdn 
		update_jdn();
	}
	// Addera n-månader
	void Date::add_month(int n) {
		// Konvertera till bas {0, 1, ... , 11} för månaderna, för att få enklare beräkningar
		int m = d_month - 1;
		int curr_months = m + n;

		// Om om vi passerar detta år (framåt) genom att addera => måste addera år
		if (curr_months > 11) {
			int add_num_years = (int) floor((float)curr_months / 12);
			d_year += add_num_years;
			m = curr_months % 12;
		}
		// Om vi adderar negativt n så kan vi passera början på året => subtrahera år
		else if (curr_months < 0) {
			int subtract_num_years = (int)-floor((float)curr_months / 12);
			d_year -= subtract_num_years;
			m = (curr_months + 12 * subtract_num_years) % 12;
		}
		// Håller vi oss inom året är det bara att addera månaden utan konstigheter
		else {
			m = curr_months;
		}
		// Konvertera tillbaka till månadsbas {1, 2, ... , 12} och uppdatera j_month
		d_month = m + 1;

		// Kolla om dagen är valid efter additionen. Om inte, sätt till sista dagen i månaden.
		if (!is_valid_day())
			d_day = days_this_month();

		// Vi har nya yy-mm-dd => uppdatera jdn
		update_jdn();

		// Om datumet inte är giltigt => throw exception
		if (!is_valid_date())
		{
			throw std::out_of_range("Error: out of range");
		}
	}
	void Date::copy(const Date &other) {
		d_year = other.d_year;
		d_month = other.d_month;
		d_day = other.d_day;
		jdn = other.jdn;
	}

	// Non-member friend function (p. 557)
	std::ostream& operator<<(std::ostream & out, const Date& d) {
		out << d.year() << "-";
		if (d.month() < 10)
			out << "0";
		out << d.month() << "-";
		
		if (d.day() < 10)
			out << "0";
		out << d.day();
		return out;
	}
}


