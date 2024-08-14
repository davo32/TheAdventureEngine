Introduction
This project utilizes GLFW and ImGui to create a graphical application in C++. Unlike web-based applications,
this project does not involve a traditional server or database. Instead, it focuses on the distinctions between
frontend and backend within the context of a desktop application.

Frontend
The frontend in this C++ project refers to the part of the application that handles the graphical user interface 
(GUI) and user interactions. It is the portion that users interact with directly.

Key Components
- GLFW: A library for creating windows, contexts, and managing input in a cross-platform manner.
- ImGui: A library for creating fast and simple graphical user interfaces with an immediate-mode GUI paradigm.
Responsibilities
- Rendering: Displaying graphical elements and the user interface.
- User Interaction: Handling input from the user, such as keyboard and mouse events.
- UI Elements: Creating buttons, windows, text fields, and other interactive elements.
- Event Handling: Responding to user actions and updating the interface accordingly.

Backend
The backend in this C++ project refers to the part of the application that handles the core functionality and logic. 
In this context, it doesn't involve server-side operations but rather the processing and management of data within the application.

Responsibilities
- Application Logic: The core functionality of the application, such as calculations, data manipulation, and state management.
- Data Handling: Managing any data required by the application, including reading from and writing to files.
- Performance Optimization: Ensuring that the application runs efficiently.

Interaction Between Frontend and Backend
In this project, the frontend and backend interact closely within the same application. 
The frontend gathers input from the user and displays information, while the backend performs the necessary
computations and logic based on that input.

Example Workflow
- User Input: The user interacts with the GUI (e.g., clicking a button or entering text).
- Frontend Processing: The frontend captures this input and sends it to the backend for processing.
- Backend Processing: The backend performs any required calculations or logic.
- Frontend Update: The frontend updates the GUI based on the results from the backend.