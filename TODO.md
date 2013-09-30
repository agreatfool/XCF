### TODO:
#### thread pool
* shall be tested, both fixed thread pool & dynamic thread pool & idle thread pool

#### socket
* put the socket buffer into socket, not expose to the outsides
* socket shall have "reading" status, socket data not all read from remote, still processing reading
* socket shall have "working" status, data received and the socket passed to worker thread to handle the business logics

#### server
* fixed the reader thead pool & worker thread pool
* reader thread: fix the read callback, if read event happened on an "working" socket, it will just pass, ignore the coming data
* reader thread: put the socket which just finished the reading process into worker thread to handle the logics
* reader thread: design the data encoding & decoding mechanism
* worker thread: design how to use a common interface to handle the socket, and process the business logics

#### log
* adjust log function, to reduce the use of Utility::stringFormat