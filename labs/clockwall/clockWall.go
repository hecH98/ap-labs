package main

import (
	"os"
	"strings"
	"net"
	"fmt"
	"io"
)

func main() {
	if len(os.Args) <= 1 {
		fmt.Print("Error, format: go run clockWall.go \"city\"=\"server\":\"port\" ...\n")
		return
	} 
	lock := make(chan int)
	for i := 1; i < len(os.Args); i++ {
		timezone := strings.Split(os.Args[i], "=")
		conn, err := net.Dial("tcp", timezone[1])
		if err != nil {
			fmt.Println(err)
		}
		go copy(conn, lock)
	}
	<- lock
}

func copy(conn net.Conn, lock chan int) {
	for {
		_, err := io.Copy(os.Stdout, conn)
		if err == nil {
			break
		}
		lock <- 1
	}
}