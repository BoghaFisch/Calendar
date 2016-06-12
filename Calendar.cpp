#include "Calendar.hpp"
//#include "gregorian.hpp"		// Inkluderat till main - behövs annars ej
//#include "julian.hpp"			// ---------------------------------------

namespace lab2 {

	// Default constructor - sets the date to the current date by invoking the default constructor for the DateType
	template<typename DateType>
	Calendar<DateType>::Calendar()
	{
		current_date = DateType();
		events = std::multimap<DateType, std::string>();
	}
	// Copy constructor - Has to iterate through the multimap, since possibly different types of dates
	template<typename DateType>
	template<typename OtherDateType >
	Calendar<DateType>::Calendar(const Calendar<OtherDateType> &other) : current_date(other.current_date) {
		events = std::multimap<DateType, std::string>();
		for (auto it = other.events.begin(); it != other.events.end(); ++it)
		{
			events.insert(std::pair<DateType, std::string>(it->first, it->second));
		}
	}
	// Destructor - Nothing special
	template<typename DateType>
	Calendar<DateType>::~Calendar()
	{
	}
	// Copy assignment
	template<typename DateType>
	template<typename OtherDateType>
	Calendar<DateType> & Calendar<DateType>::operator=(const Calendar<OtherDateType> &other) {
		current_date = other.current_date;
		events = std::multimap<DateType, std::string>();
		for (auto it = other.events.begin(); it != other.events.end(); ++it)
		{
			events.insert(std::pair<DateType, std::string>(it->first, it->second));
		}
		return *this;
	}
	// Sets the current date of the calendar. Return true if the date is valid, otherwise false.
	template<typename DateType>
	bool Calendar<DateType>::set_date(int y, unsigned m, unsigned d) {
		current_date = DateType(y, m, d);
		if (current_date.is_valid_date())
			return true;
		else return false;
	}
	// Adds an event to the calendar. Default params are current date. Returns true if the date was inserted, otherwise false.
	template<typename DateType>
	bool Calendar<DateType>::add_event(std::string event_name) {
		return add_event(event_name, current_date.day(), current_date.month(), current_date.year());
	}
	template<typename DateType>
	bool Calendar<DateType>::add_event(std::string event_name, int d) {
		return add_event(event_name, d, current_date.month(), current_date.year());
	}
	template<typename DateType>
	bool Calendar<DateType>::add_event(std::string event_name, int d, int m) {
		return add_event(event_name, d, m, current_date.year());
	}
	template<typename DateType>
	bool Calendar<DateType>::add_event(std::string event_name, int d, int m, int y) {
		DateType date(y, m, d);

		// ret is a pair representing the range [first, last] of the events corresponding to the date searched for
		std::pair <std::multimap<DateType, std::string>::iterator, std::multimap<DateType, std::string>::iterator> ret = events.equal_range(date);
		
		// Check if event already exists
		while (ret.first != ret.second) {
			if ((ret.first)->second == event_name)
				return false;
			ret.first++;
		}

		// If event didn't aleready exist - insert it and return true
		events.insert(std::pair<DateType, std::string>(date, event_name));
		return true;
	}
	// Removes an event from the calendar. True if successfully removed, otherwise false. Default params are current date.
	template<typename DateType>
	bool Calendar<DateType>::remove_event(std::string event_name) {
		return remove_event(event_name, current_date.day(), current_date.month(), current_date.year());
	}
	template<typename DateType>
	bool Calendar<DateType>::remove_event(std::string event_name, int d) {
		return remove_event(event_name, d, current_date.month(), current_date.year());
	}
	template<typename DateType>
	bool Calendar<DateType>::remove_event(std::string event_name, int d, int m) {
		return remove_event(event_name, d, m, current_date.year());
	}
	template<typename DateType>
	bool Calendar<DateType>::remove_event(std::string event_name, int d, int m, int y) {
		DateType date(y, m, d);

		// ret is a pair representing the range [first, last] of the events corresponding to the date searched for
		std::pair <std::multimap<DateType, std::string>::iterator, std::multimap<DateType, std::string>::iterator> ret = events.equal_range(date);

		// Check if event exists. If it does - remove it and return true
		while (ret.first != ret.second) {
			if ((ret.first)->second == event_name)	{
				events.erase(ret.first);
				return true;
			}
			ret.first++;
		}
		// If none were found - return false
		return false;
	}
	template<typename DateType>
	void Calendar<DateType>::print_readably() {
		for (auto it = events.begin(); it != events.end(); ++it) {
			std::cout << (*it).first << " => " << (*it).second << std::endl;
		}
	}
	// Prints the calendar in iCal-format
	template <typename DateType>
	std::ostream & operator<<(std::ostream & os, const Calendar<DateType> & calendar) {
		/*
		BEGIN:VCALENDAR
		VERSION:2.0
		PRODID:-//hacksw/handcal//NONSGML v1.0//EN
		BEGIN:VEVENT
		UID:uid1@example.com
		DTSTAMP:19970714T170000Z
		ORGANIZER;CN=John Doe:MAILTO:john.doe@example.com
		DTSTART:19970714T170000Z
		DTEND:19970715T035959Z
		SUMMARY:Bastille Day Party
		END:VEVENT
		END:VCALENDAR
		*/
		os << "BEGIN:VCALENDAR" << std::endl;
		for (auto it = calendar.events.begin(); it != calendar.events.end(); ++it)
		{
			if ((*it).first >= calendar.current_date) {
				os << "BEGIN:VEVENT" << std::endl;
				os << "VERSION:2.0" << std::endl;
				os << "DTSTART:";
				os << ((*it).first).year();
				if (((*it).first).month() < 10)
					os << "0";
				os << ((*it).first).month();
				if (((*it).first).day() < 10)
					os << "0";
				os << ((*it).first).day() << "T100000Z" << std::endl;
				os << "SUMMARY:" << (*it).second << std::endl;
				os << "END:VEVENT" << std::endl;
			}
		}
		os << "END:VCALENDAR" << std::endl;
		return os;
	}
}
int main() {
	/*using namespace lab2;
	using namespace std;
	Calendar<Gregorian> cal;
	cal.set_date(2000, 12, 2);
	cal.add_event("Basketträning", 4, 12, 2000);
	cal.add_event("Basketträning", 11, 12, 2000);
	cal.add_event("Nyårsfrukost", 1, 1, 2001);
	cal.add_event("Första advent", 1);          // år = 2000, månad = 12
	cal.add_event("Vårdagjämning", 20, 3);      // år = 2000
	cal.add_event("Julafton", 24, 12);
	cal.add_event("Kalle Anka hälsar god jul", 24); // också på julafton
	cal.add_event("Julafton", 24); // En likadan händelse samma datum ska
								   // ignoreras och inte sättas in i kalendern
	cal.add_event("Min första cykel", 20, 12, 2000);
	cal.remove_event("Basketträning", 4);
	std::cout << cal; // OBS! Vårdagjämning och första advent är
					  // före nuvarande datum och skrivs inte ut
	std::cout << "----------------------------------------" << std::endl;
	cal.remove_event("Vårdagjämning", 20, 3, 2000);
	cal.remove_event("Kalle Anka hälsar god jul", 24, 12, 2000);
	cal.set_date(2000, 11, 2);
	if (!cal.remove_event("Julafton", 24)) {
		std::cout << " cal.remove_event(\"Julafton\", 24) tar inte" << std::endl
			<< " bort något eftersom aktuell månad är november" << std::endl;
	}
	std::cout << "----------------------------------------" << std::endl;
	//std::cout << cal;
	cal.print_readably();*/
	using namespace lab2;
	Calendar<Gregorian> gcal;
	gcal.add_event("ASDF", 2012, 2, 29);
}
