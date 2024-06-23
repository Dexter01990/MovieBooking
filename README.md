# Movie Booking Service

This is a backend service for booking online movie tickets implemented in C++.

## Features

- View all playing movies.
- See all theaters showing a movie.
- See available seats for the selected theater and movie.
- Book one or more of the available seats.
- Handles multiple requests simultaneously to prevent over-bookings.

## Requirements

- C++11 or later

## How to Compile and Run

1. Open a terminal.
2. Navigate to the directory containing the source code.
3. Compile the code using the following command:
    ```sh
    g++ -std=c++11 main.cpp -o movie_booking
    ```
4. Run the compiled program:
    ```sh
    ./movie_booking
    ```

## Usage

Follow on-screen menu to interact with the service. You can view movies, see theaters showing a movie, check available seats, and book seats.

### Booking Seats

When prompted to enter the theater name, ensure that you enter a valid theater name. If you enter an invalid theater name, you will be notified with an error message.

When booking seats, enter the seat numbers as shown (1-based indexing). The program will handle the conversion to the internal 0-based indexing. If an invalid or unavailable seat number is entered, the program will notify you with an error message.

### Menu Options

1. **View all playing movies**
   - Displays a list of all movies currently playing.
2. **See all theaters showing a movie**
   - Ask you to enter the movie name and displays a list of theaters showing the movie.
3. **See available seats for a selected theater & movie**
   - Prompts you to enter the theater name and displays a list of available seats in the theater.
4. **Book one or more seats**
   - Ask you to enter the theater name and the number of seats to book.
   - Then prompts you to enter the seat numbers.
   - If the seat numbers are valid and available, books the seats and confirms the booking.
   - If any seat number is invalid or unavailable, notifies you with an error message.
5. **Exit**
   - Exits the program.

### Detailed Example

1. **View all playing movies:**
   - Choose option 1 from the menu.
   - The program will display all the movies currently playing.

2. **See all theaters showing a movie:**
   - Choose option 2 from the menu.
   - Enter the name of the movie.
   - The program will display all the theaters showing that movie.

3. **See available seats for a selected theater & movie:**
   - Choose option 3 from the menu.
   - Enter the name of the theater.
   - The program will display all available seats in that theater.

4. **Book one or more seats:**
   - Choose option 4 from the menu.
   - Enter the name of the theater.
   - Enter the number of seats you want to book.
   - Enter the seat numbers you want to book.
   - The program will check the validity of the seat numbers and book the seats if they are available.
   - If any seat number is invalid or unavailable, the program will notify you with an error message.
**API Documentation:**
# Theater Class
# Theater(const std::string& name)
Constructor to initialize a theater with a name and 20 available seats.

# const std::string& getName() const
Returns the name of the theater.

# std::vector<int> getAvailableSeats() const
Returns a list of available seat numbers.

# bool bookSeats(const std::vector<int>& seatsToBook)
Books the specified seats if they are available. Returns true if successful, false otherwise.

## MovieService Class
## void addMovie(const std::string& movieName, const std::vector<std::string>& theaterNames)
Adds a movie and the theaters where it is being shown.

## std::vector<std::string> getAllMovies() const
Returns a list of all movies.

## std::vector<std::string> getTheatersForMovie(const std::string& movieName) const
Returns a list of theaters showing the specified movie.

## std::vector<int> getAvailableSeats(const std::string& theaterName) const
Returns a list of available seats for the specified theater.

## bool bookSeats(const std::string& theaterName, const std::vector<int>& seatsToBook)
Books the specified seats in the specified theater. Returns true if successful, false otherwise.

## bool isValidSeatNumber(int seat, const std::vector<int>& availableSeats) const
Checks if the seat number is valid and available.

## Reflection

### What aspect of this exercise did you find the most interesting?
Implementing the seat booking functionality while ensuring thread safety was quite interesting. It required careful consideration of concurrent access and potential race conditions.

### What did you find most cumbersome?
Managing the interactions through the CLI and ensuring a good user experience without a persistent database was somewhat cumbersome. Additionally, handling input validation and error messages required extra effort.

### Bonus Features

- [ ] Conan package management
- [ ] Docker support
