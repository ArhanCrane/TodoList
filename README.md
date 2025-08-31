A RESTful API to allow users to manage their to-do list.
There's a cpp file of a command-line interface in the CLI folder.

---
Components:
- CLI - C++, libcurl library
- API - Userver, postgresql
---

Three http handlers:
1. /todos <br>
	Methods: GET, POST, DELETE, PUT <br>
	GET: localhost:PORT/todos?name=NAME&title=TITLE <br>
	This method returns the title and the value as an array.

	POST: localhost:PORT/todos?name=NAME&title=TITLE&value=VALUE <br>
	This POST request inserts the login, the title and the value as a new line into hello_schema.items.

	DELETE: localhost:PORT/todos?name=NAME&title=TITLE <br>
	It deletes the line with a certain title where the name = NAME.

	PUT: localhost:PORT/todos?
	name=NAME&title=TITLE&value=VALUE <br>
	The PUT request updates the note of a certain name replacing the value with a particular one.

2. /register <br>
	Methods: POST <br>
	POST: localhost:PORT/register?name=NAME&password=PASSWORD <br>
	user-registration-handler put the name and the password into hello_schema.users

3. /login <br>
	Methods: GET <br>
	GET: localhost:PORT/login?name=NAME&password=PASSWORD <br>
	user-login-handler takes a password if it exists in a database and compare it to the password in a GET request. If these two passwords are equal then the user is getting an access to their notes.
