# WebServer Analysis

- The HttpServer thread/object should only accept requests and queue them, nothing more.

- Threads/HttpConnectionHandler objects consume connections.

- HttpConnectionHandler threads/objects fetch all requests from the client (not just the first one). For each request, assemble the HttpRequest and HttpResponse objects.

- The HttpConnectionHandler is a server layer object, not an application layer. For every request you pull, you don't have to serve it directly, but delegate to web applications. That is, the HttpConnectionHandler will go through all the web applications in search of the one in charge of serving the request (if there is one). For this preview the web application is just an object, not a thread of execution.

- In this project, the web application is an object that will receive the request (the HttpRequest and HttpResponse objects) and compute the Goldbach sums of each requested number in the same thread of execution as the HttpConnectionHandler.

# WebApp analysis

Before, a concurrent web server was built that could await an arbitrage bid from concurrent form clients, who would request to get the Goldbach integer sums, which is computed for a serial web application. However, this designer is not optimal. For example, if a web server capable of waiting for 1000 concurrent connections arises on a machine with 8 processor cores, you can find scenarios such as the following:

If indeed 1000 users are connected requesting a considerable amount of work each, the server will have 1000 threads (or "1000 calculators") competing for all 8 cores, creating a lot of competition and context switching for them, plus increased consumption of main memory.

If you connect several users, one of them makes a request that requires a considerable amount of work, say an hour, and the other requests make light ones. The threads serving light requests will kill fast, while the thread handling the heavy request will be left working for a long time on one CPU core, while the remaining seven CPUs will be wasted.

In this advance, a solution will be created that will make the best use of the machine's resources, to work concurrently with the web application that calculates the Goldbach sums. The application will create as many calculators from Goldbach sums as there are CPUs available in the system. If the development team wants it, they can provide a third command line argument that controls this many calculators, but it is not required. The amount of additions is a detail that should be added to the readme.ext of the project.

## Httpperf analysis

The Httpperf tool is a tool that allows you to test the performance of a web server. The following parameters were used: 

    httperf --server localhost --port 8080 --uri /Goldbach?number=27777,1234 --num-conns 400 --rate 50 --num-call 10 --timeout 10

The results were as follows:

![Httperf](/project1.2/img/httpperf.png)

## Flow Diagram Analysis

![Diagram](/project1.2/img/flowDiagramDesignConcurrentServer.png)

- After getting the raw decomposition of the numbers from a request, the web application will respond directly to the client using the socket, but without closing it (unless the client uses the HTTP/1.0 protocol).

- Once the web application responds to each client available, the HttpConnectionHandler of that client returns to the request cycle to service any further requests for GoldbachWebApp, that passes the processed given numbers of the client to GoldbachSolvers which also return the Goldbach sums of each number given via a Goldbach calculator, that the client may have. This process works parallelly to any given connection at the time and uses a singleton pattern of the GoldbachWebApp.

- All the solvers will then use a single GoldbachPacker to create the whole struct that will be used by a single HttpDispatcher to send the response back to the user.

- When each individual client has no more requests, the connection (socket) is closed.
