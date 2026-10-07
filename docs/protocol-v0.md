pigeite compute is a distrobuted compute project for which protocol v0 exsists to support

the goal of v0 is to show a distrobuted monte carlo pi calaculation across multiple nodes.

protocol v0 will be very small and simple and the basic communication model is:

NODE -> SERVER             TCP connection
NODE -> SERVER              HELLO
SERVER -> NODE              HELLO_ACK
NODE -> SERVER             CAPABILITYS
NODE -> SERVER             READY
SERVER -> NODE             WORK_OFFER
NODE -> SERVER              WORK_ACCEPT

NODE -> SERVER             WORK_RESULT
SERVER -> NODE             RESULLT_ACEPT

NODE -> SERVER             READY
SERVER -> NODE             NEXT WORK_OFFER



protocol v0 will have to contain 
 -tcp communication between node and server
 -node capability reporting
 -node readiness reporting
 -work offers
 -accepting and rejecting work
 -monte carlo pi work units
 -reslt submission
 -result acknowledgement
 -basic heartbeats
 -error reporting
 -clean disconection
 

 but protocol v0 wil not contain

 -user accounts
 -encryption
 -authentication
 -gpu workloads
 -webassembley
 -checkpoint transfer
 -project selection
 -binary protocol enconding
 -compression

 these features may be introduced in later protocol versions

 protocol v0 uses TCP and the inital development port will be 31820

 protocol v0 uses UTF-8 encoded JSON and will consist of exactly oone JSON object followed by a newline charachter.

 a node follows the connection lifecycle of:

 connect
 handshake
 registerd
 idle
 work offerd
    reject ----- idle
    accept

    working
    submit result
    idle

a node can repeat the idle working idle cycle many times durng one connection

**protocall runthrough**

1) **HELLO**
imediatly affter establishing a TCP connection a node sends hello, this looks like

{
    "type": "hello",
    "protocol": 0,
    "client": "pigeite-node",
    "client_version": "0.0.1"
}

field          type     description

type           string   must be hello
protocol       integer  protocol version
client         string   client
client_version string   client software versiopn

2) **HELLO_ACK**
if the server supports the protocol version it sends HELLO_ACK this looks like

{
    "type": "hello_ack",
    "protocol": 0,
    "server_version": "0.0.1",
    "node_id": "n000003"
}

im too lazy to do another table thing its self explanatory

3) **CAPABILITIES**
after the handshake the node send information about all the resources it can provide this looks like

{
    "type": "capabilities",
    "architecture": "x86_64",
    "logical_threads": 16,
    "memory_mb": 8192,
    "os": "windows",
    "workloads": [
        "pi.monte_carlo"
    ]
}

the capabilitys message shows the rsources alocated to pigeite not the total amount of resources