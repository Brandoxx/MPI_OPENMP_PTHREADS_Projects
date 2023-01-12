# WebServer Analysis

- The HttpServer thread/object should only accept requests and queue them, nothing more.

- Threads/HttpConnectionHandler objects consume connections.

- HttpConnectionHandler threads/objects fetch all requests from the client (not just the first one). For each request, assemble the HttpRequest and HttpResponse objects.

- The HttpConnectionHandler is a server layer object, not an application layer. For every request you pull, you don't have to serve it directly, but delegate to web applications. That is, the HttpConnectionHandler will go through all the web applications in search of the one in charge of serving the request (if there is one). For this preview the web application is just an object, not a thread of execution.

- In this project, the web application is an object that will receive the request (the HttpRequest and HttpResponse objects) and compute the Goldbach sums of each requested number in the same thread of execution as the HttpConnectionHandler.

## Flow Diagram Analysis

![Diagram](/project1.1/img/flowDiagramDesignConcurrentServer.png)

- After getting the raw decomposition of the numbers from a request, the web application will respond directly to the client using the socket, but without closing it (unless the client uses the HTTP/1.0 protocol).

- Once the web application responds to each client available, the HttpConnectionHandler of that client returns to the request cycle to service any further requests for GoldbachWebApp, that passes the processed given numbers of the client to GoldbachCalculator which also returns the Goldbach sums of each number given, that the client may have. This process works parallelly to any given connection at the time and uses a singleton pattern of the GoldbachWebApp.

- When each individual client has no more requests, the connection (socket) is closed.
