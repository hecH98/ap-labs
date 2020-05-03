// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"flag"
	"strings"
	"time"
)

type usuario struct {
	name string
	admin bool
	ip string
	canal chan string
	connection net.Conn
}

var connectedUsers int 
var allUsers []usuario
//!+broadcaster
type client chan<- string // an outgoing message channel

var (
	entering = make(chan client)
	leaving  = make(chan client)
	messages = make(chan string) // all incoming client messages
)

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

func showUsers() string{
	// fmt.Println(len(allUsers))
	usrs := "irc-server > "
	for user := range allUsers {
		// fmt.Println(allUsers[user])
		usrs += allUsers[user].name + ", "
	}
	// fmt.Println(usrs)
	return usrs
}

func searchUser(usuario string) string{
	// fmt.Println(len(allUsers))
	usr := "irc-server > "
	found := false
	for user := range allUsers {
		if allUsers[user].name == usuario {
			usr += "username: " + allUsers[user].name + ", IP: " + allUsers[user].ip
			found = true
			break
		}
		// fmt.Println(allUsers[user])
		
	}
	if !found {
		usr += "user not found"
	}
	// fmt.Println(usr)
	return usr
}

func getTime() string{
	time := "irc-server > Local Time: America/Mexico_City " + time.Now().Format("15:04:05")
	return time
}

func messageUser(msgFrom, msgTo, msg string){
	for user := range allUsers {
		if allUsers[user].name == msgTo {
			allUsers[user].canal <- "irc-server > Message from [" + msgFrom + "]: " + msg
			// fmt.Println(allUsers[user])
			break
		}
	}
}

func kickUser(usuario string){
	for user := range allUsers {
		if allUsers[user].name == usuario {
			fmt.Println("irc-server > [" + usuario + "] was kicked")
			allUsers[user].canal <- "You have been kicked for offensive language"
			time.Sleep(1 * time.Second)
			allUsers[user].connection.Close()
			
			break
		}
	}
}

//!+handleConn
func handleConn(conn net.Conn) {
	// fmt.Println("len(allUsers)")
	// fmt.Println(len(allUsers))
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)

	input := bufio.NewScanner(conn)
	input.Scan()
	who := usuario {
		name : input.Text(),
		admin : (connectedUsers == 0),
		ip : conn.RemoteAddr().String(),
		canal : ch,
		connection : conn,
	}
	connectedUsers++
	fmt.Println("irc-server > New connected user [" + who.name + "]")

	allUsers = append(allUsers, who)

	ch <- "irc-server > Welcome to the Simple IRC Server"
	ch <- "irc-server > Your user [" + who.name + "] is successfully logged"
	if who.admin {
		fmt.Println("irc-server > [" + who.name + "] was promoted as the channel ADMIN")
		ch <- "irc-server > Congrats, you were the first user."
		ch <- "irc-server > You're the new IRC Server ADMIN"
	}
	messages <- "irc-server > New connected user [" + who.name + "]"
	entering <- ch
	for input.Scan() {
		texto := input.Text()
		comando := strings.SplitN(texto, " ", 3)
		// fmt.Println(comando[0])
		switch comando[0] {
		case "/users":
			// fmt.Println("check users")
			ch <- showUsers()
		case "/msg":
			messageUser(who.name, comando[1], comando[2])
			// fmt.Println("message to someone")
		case "/time":
			ch <- getTime()
			// fmt.Println("see time")
		case "/user":
			ch <- searchUser(comando[1])
			// fmt.Println("info user")
		case "/kick":
			if who.admin {
				// time.Sleep(1 * time.Second)
				kickUser(comando[1])
				messages <- "irc-server > [" + comando[1] + "] has been kicked for offensive language"
				// fmt.Println("kick user")
			} else {
				ch <- "Cannot kick user, not server admin"
			}
			
		default:
			messages <- who.name + " > " + input.Text()
		}
		
	}
	// NOTE: ignoring potential errors from input.Err()

	leaving <- ch
	messages <- "irc-server > [" + who.name + "] left channel"
	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

// go run server.go -host localhost -port 9000
//!+main
func main() {
	numbers := make([]int, 0)
	numbers = append(numbers, 1)
	numbers = append(numbers, 2)
	numbers = append(numbers, 5)
	// fmt.Println(len(numbers)) // == 2
	// for number := range numbers {
	// 	// fmt.Println(numbers[number]) // == 2
	// }
	// for clie := 0; clie < len(numbers); clie++ {
	// 	// fmt.Println(numbers[clie])
	// }
	
	connectedUsers = 0
	host := flag.String("host", "", "host")
	port := flag.String("port", "", "port")
	flag.Parse()
	// fmt.Println("host value ", *host)
	// fmt.Println("port value ", *port)
	var puerto = *host + ":" + *port
	// fmt.Println("valor 2 ", puerto)
	listener, err := net.Listen("tcp", puerto)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("irc-server > Simple IRC Server started at",puerto)
	fmt.Println("irc-server > Ready for receiving new clients")
	go broadcaster()
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

//!-main
