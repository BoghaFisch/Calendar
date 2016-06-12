#ifndef CALENDAR_HPP
#define CALENDAR_HPP
#include "date.hpp"
#include "gregorian.hpp"
#include "julian.hpp"
#include <string>
#include <map>
#include <iostream>


namespace lab2 {
	template<typename DateType>
	class Calendar
	{
	private:
		DateType current_date;
		std::multimap<DateType, std::string> events;
		template<typename OtherDateType> friend class Calendar;

	public:
		// Constructors
		Calendar();

		// Copy constructor
		template<typename OtherDateType>
		Calendar(const Calendar<OtherDateType> &other);

		// Destructor
		~Calendar();

		// Operators
		template<typename OtherDateType>
		Calendar<DateType> & operator=(const Calendar<OtherDateType> &other);

		// Member functions
		bool set_date(int y, unsigned m, unsigned d);
		bool add_event(std::string event_name);
		bool add_event(std::string event_name, int);
		bool add_event(std::string event_name, int, int);
		bool add_event(std::string event_name, int, int, int);
		bool remove_event(std::string event_name);
		bool remove_event(std::string event_name, int);
		bool remove_event(std::string event_name, int, int);
		bool remove_event(std::string event_name, int, int, int);

		// Print
		template<typename DateType>
		friend std::ostream& operator<<(std::ostream &, const Calendar<DateType> &);

		// Return methods for debugging
		std::multimap<DateType, std::string> get_events() { return events;  }
		DateType get_curr_date() { return current_date;  }
		void print_readably();
	};
}
#endif
