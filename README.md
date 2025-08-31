A RESTful API to allow users to manage their to-do list.
There's a cpp file of a command-line interface in the CLI folder.

---
Three http handlers:
1. /todos
Methods: GET, POST, DELETE, PUT
GET: localhost:PORT/todos?name=NAME&title=TITLE
This method returns the title and the value as an array.

POST: localhost:PORT/todos?name=NAME&title=TITLE&value=VALUE
This POST request inserts the login, the title and the value as a new line into hello_schema.items.

DELETE: localhost:PORT/todos?name=NAME&title=TITLE
It deletes the line with a certain title where the name = NAME.

PUT: localhost:PORT/todos?
name=NAME&title=TITLE&value=VALUE
The PUT request updates the note of a certain name replacing the value with a particular one.

2. /register
Methods: POST
POST: localhost:PORT/register?name=NAME&password=PASSWORD
user-registration-handler put the name and the password into hello_schema.users

3. /login
Methods: GET
GET: localhost:PORT/login?name=NAME&password=PASSWORD
user-login-handler takes a password if it exists in a database and compare it to the password in a GET request. If these two passwords are equal then the user is getting an access to their notes.
