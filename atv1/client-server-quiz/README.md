This project implements a simple client-server application that conducts a multiple-choice quiz. The server sends two questions to the client, evaluates the responses, and returns the results.

## Project Structure

```
client-server-quiz
├── server.py       # Server-side logic
├── client.py       # Client-side logic
└── README.md       # Project documentation
```

## How to Run the Project

### Prerequisites

Make sure you have Python installed on your machine. You can download it from [python.org](https://www.python.org/downloads/).

### Running the Server

1. Open a terminal or command prompt.
2. Navigate to the project directory:
   ```
   cd path/to/client-server-quiz
   ```
3. Run the server:
   ```
   python server.py
   ```

### Running the Client

1. Open another terminal or command prompt.
2. Navigate to the project directory:
   ```
   cd path/to/client-server-quiz
   ```
3. Run the client:
   ```
   python client.py
   ```

## Quiz Functionality

- The server will send two multiple-choice questions to the client.
- The client will prompt the user to answer the questions.
- After the user submits their answers, the client sends them back to the server.
- The server evaluates the answers and returns the number of correct answers along with a list indicating which answers were correct or incorrect.

Enjoy the quiz!