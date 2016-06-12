#ifndef DATE_HPP
#define DATE_HPP
#define VISUAL_STUDIO 0

#if VISUAL_STUDIO == 1
#include "kattistime.h"
#endif

#include <string>
#include <iostream>
#include <stdexcept>
#include <ctime>
#include <cmath>

namespace lab2 {

	static const std::string week_day_names[] = { "INDEX_0", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday" };
	static const std::string month_names[] = { "INDEX_0","january", "february", "march", "april", "may", "june", "july", "august", "september", "october" , "november", "december" };
	static const unsigned days_per_month[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	class Date
	{
	private:
		virtual int calculate_jdn(int, unsigned, unsigned) const = 0;
		virtual void update_jdn() = 0;
		virtual void refresh() = 0;
		virtual bool is_leap_year() const = 0;

	protected:
		int jdn, d_year;
		unsigned d_month, d_day;

	public:
		// Constructors
		Date();

		// Destructor
		virtual ~Date();

		// Copy assignment
		virtual Date & operator=(const Date &);

		// Inline Methods
		inline int get_min_year() const { return 1858; }
		inline int get_max_year() const { return 2558; }
		inline unsigned days_per_week() const { return 7; }
		inline int get_jdn() const { return jdn; }
		inline int year() const { return d_year; }
		inline unsigned month() const { return d_month; }
		inline unsigned day() const { return d_day; }
		inline int mod_julian_day() const { return jdn - 2400001; }		// Rundat från 24000000.5 eftersom vi inte jobbar med timmar 				
		
		// Comparators
		inline bool operator==(const Date &other) const { return jdn == other.jdn; }
		inline bool operator!=(const Date &other) const { return jdn != other.jdn; }
		inline bool operator<(const Date &other) const { return jdn < other.jdn; }
		inline bool operator<=(const Date &other) const { return jdn <= other.jdn; }
		inline bool operator>(const Date &other) const { return jdn > other.jdn; }
		inline bool operator>=(const Date &other) const { return jdn >= other.jdn; }

		// Motifier-operators
		Date & operator++();
		Date & operator--();
		Date & operator+=(int);
		Date & operator-=(int);
		inline int operator-(const Date & other) const { return jdn - other.jdn; }
		friend std::ostream& operator<<(std::ostream &, const Date &);

		// Methods
		std::string week_day_name() const;
		std::string month_name() const;
		unsigned days_this_month() const;
		unsigned week_day() const;
		void add_year(int n = 1);
		void add_month(int n = 1);
		void copy(const Date &);

		bool is_valid_day() const;
		bool is_valid_month() const;
		bool is_valid_year() const;
		bool is_valid_date() const;
	};
}
#endif
