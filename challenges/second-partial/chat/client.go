// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 227.

// Netcat is a simple read/write client for TCP servers.
package main

import (
	"io"
	"log"
	"net"
	"os"
	"fmt"
	"flag"
)
// go run client.go -user user2 -server localhost:9000
//!+

func main() {
	server := flag.String("server", "", "host:port")
	user := flag.String("user", "", "username")
	flag.Parse()

	fmt.Println("user value ", *user)
	fmt.Println("server value ", *server)
	conn, err := net.Dial("tcp", *server)
	if err != nil {
		log.Fatal(err)
	}
	enterUser := true
	done := make(chan struct{})
	go func() {
		if enterUser {
			fmt.Print("Enter your username: " + *user + " > ")
			enterUser = false
		}
		io.Copy(os.Stdout, conn) // NOTE: ignoring errors
		log.Println("done")
		done <- struct{}{} // signal the main goroutine
	}()
	fmt.Println("esperando mensaje")
	mustCopy(conn, os.Stdin)
	conn.Close()
	<-done // wait for background goroutine to finish
}

//!-

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}
