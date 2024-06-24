#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <mutex>
#include <memory>
#include <algorithm>
#include <sstream>
#include <limits>
#include <set>

class Theater {
public:
	Theater(const std::string& name) : name(name), seats(20, true) {}

	const std::string& getName() const {
		return name;
	}

	std::vector<int> getAvailableSeats() const {
		std::lock_guard<std::mutex> lock(mutex);
		std::vector<int> availableSeats;
		for (int i = 0; i < seats.size(); ++i) {
			if (seats[i]) {
				availableSeats.push_back(i + 1); // Return 1-based seat numbers
			}
		}
		return availableSeats;
	}

	bool bookSeats(const std::vector<int>& seatsToBook) {
		std::lock_guard<std::mutex> lock(mutex);
		for (int seat : seatsToBook) {
			if (seat < 0 || seat >= seats.size() || !seats[seat]) {
				return false;
			}
		}
		for (int seat : seatsToBook) {
			seats[seat] = false; // Mark as booked
		}
		return true;
	}

private:
	std::string name;
	std::vector<bool> seats;
	mutable std::mutex mutex;
};

class MovieService {
public:
	void addMovie(const std::string& movieName, const std::vector<std::string>& theaterNames) {
		for (const std::string& theaterName : theaterNames) {
			theaters[theaterName] = std::make_shared<Theater>(theaterName);
		}
		movies[movieName] = theaterNames;
	}

	std::vector<std::string> getAllMovies() const {
		std::vector<std::string> movieNames;
		for (const auto& pair : movies) {
			movieNames.push_back(pair.first);
		}
		return movieNames;
	}

	std::vector<std::string> getTheatersForMovie(const std::string& movieName) const {
		auto it = movies.find(movieName);
		if (it != movies.end()) {
			return it->second;
		}
		return {};
	}

	std::vector<int> getAvailableSeats(const std::string& theaterName) const {
		auto it = theaters.find(theaterName);
		if (it != theaters.end()) {
			return it->second->getAvailableSeats();
		}
		return {};
	}

	bool bookSeats(const std::string& theaterName, const std::vector<int>& seatsToBook) {
		auto it = theaters.find(theaterName);
		if (it != theaters.end()) {
			return it->second->bookSeats(seatsToBook);
		}
		return false;
	}

	bool isValidSeatNumber(int seat, const std::vector<int>& availableSeats) const {
		return std::find(availableSeats.begin(), availableSeats.end(), seat) != availableSeats.end();
	}

private:
	std::unordered_map<std::string, std::vector<std::string>> movies;
	std::unordered_map<std::string, std::shared_ptr<Theater>> theaters;
	mutable std::mutex mutex;
};

void showMenu() {
	std::cout << "\n1. View all playing movies\n";
	std::cout << "2. See all theaters showing a movie\n";
	std::cout << "3. See available seats for a selected theater & movie\n";
	std::cout << "4. Book one or more seats\n";
	std::cout << "5. Exit\n";
	std::cout << "Enter your choice: ";
}

bool getInputAsInteger(int& input) {
	std::string line;
	if (!std::getline(std::cin, line)) {
		return false;
	}
	std::istringstream iss(line);
	return (iss >> input) && (iss.eof());
}

int main() {
	MovieService service;
	service.addMovie("Movie1", { "Theater1", "Theater2" });
	service.addMovie("Movie2", { "Theater1", "Theater3" });

	while (true) {
		showMenu();
		int choice;
		if (!getInputAsInteger(choice)) {
			std::cout << "Invalid input. Please enter a number.\n";
			continue;
		}
		if (choice == 5) break;

		switch (choice) {
		case 1: {
			std::vector<std::string> movies = service.getAllMovies();
			if (movies.empty()) {
				std::cout << "No movies available.\n";
			}
			else {
				for (const std::string& movie : movies) {
					std::cout << movie << "\n";
				}
			}
			break;
		}
		case 2: {
			std::string movieName;
			std::cout << "Enter movie name: ";
			std::getline(std::cin, movieName);
			std::vector<std::string> theaters = service.getTheatersForMovie(movieName);
			if (theaters.empty()) {
				std::cout << "Invalid movie name or no theaters showing this movie.\n";
			}
			else {
				for (const std::string& theater : theaters) {
					std::cout << theater << "\n";
				}
			}
			break;
		}
		case 3: {
			std::string theaterName;
			std::cout << "Enter theater name: ";
			std::getline(std::cin, theaterName);
			std::vector<int> seats = service.getAvailableSeats(theaterName);
			if (seats.empty()) {
				std::cout << "Invalid theater name or no available seats.\n";
			}
			else {
				for (int seat : seats) {
					std::cout << "Seat " << seat << " is available\n";
				}
			}
			break;
		}
		case 4: {
			std::string theaterName;
			std::cout << "Enter theater name: ";
			std::getline(std::cin, theaterName);
			std::vector<int> availableSeats = service.getAvailableSeats(theaterName);
			if (availableSeats.empty()) {
				std::cout << "Invalid theater name or no available seats.\n";
				break;
			}

			int numSeats;
			std::cout << "Enter number of seats to book: ";
			if (!getInputAsInteger(numSeats) || numSeats < 1) {
				std::cout << "Invalid input. Please enter a valid number of seats.\n";
				break;
			}
			if (numSeats > availableSeats.size()) {
				std::cout << "Error: Number of seats requested exceeds available seats.\n";
				break;
			}

			std::vector<int> seatsToBook;
			std::set<int> uniqueSeats;
			std::cout << "Enter seat numbers: ";
			for (int i = 0; i < numSeats; ++i) {
				int seat;
				if (!getInputAsInteger(seat)) {
					std::cout << "Invalid input. Please enter valid seat numbers.\n";
					--i; // To prompt the user again for the same seat
					continue;
				}
				else if (seat < 1 || seat > 20) { // Check if seat number is within the valid range
					std::cout << "Error: Seat " << seat << " is out of valid range.\n";
					--i; // To prompt the user again for the same seat
					continue;
				}
				else if (!service.isValidSeatNumber(seat, availableSeats)) {
					std::cout << "Error: Seat " << seat << " is not available.\n";
					--i; // To prompt the user again for the same seat
					continue;
				}
				else if (uniqueSeats.find(seat) != uniqueSeats.end()) {
					std::cout << "Error: Seat " << seat << " has already been entered.\n";
					--i; // To prompt the user again for the same seat
					continue;
				}
				else {
					uniqueSeats.insert(seat);
					seatsToBook.push_back(seat); // Keep 1-based index for booking
				}
			}

			// Convert seatsToBook to 0-based index before booking
			std::vector<int> zeroBasedSeatsToBook;
			for (int seat : seatsToBook) {
				zeroBasedSeatsToBook.push_back(seat - 1);
			}

			if (service.bookSeats(theaterName, zeroBasedSeatsToBook)) {
				std::cout << "Seats booked successfully\n";
			}
			else {
				std::cout << "Failed to book seats. Some seats may have already been booked.\n";
			}
			break;
		}
		default:
			std::cout << "Invalid choice\n";
		}
	}

	return 0;
}
