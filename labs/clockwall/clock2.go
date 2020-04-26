// Clock2 is a concurrent TCP server that periodically writes the time.
package main

import (
	"os"
	"fmt"
	"io"
	"log"
	"net"
	"time"
)

func handleConn(c net.Conn) {
	defer c.Close()
	_, err := time.LoadLocation(os.Getenv("TZ"))
	if err != nil {
		fmt.Println(err)
	}
	for {
		_, err := io.WriteString(c, os.Getenv("TZ") + " : " + time.Now().Format("15:04:05\n"))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {
	if len(os.Args) < 1 {
		fmt.Print("Error, format: TZ=\"timezone\" go run clock2.go  -port \"port\" & ...\n")
		return
	} 

	listener, err := net.Listen("tcp", "localhost:" + os.Args[2])
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn) // handle connections concurrently
	}
}