package main

import (
	"fmt"
	"os"
	"path/filepath"
)

// scanDir stands for the directory scanning implementation
func scanDir(dir string) error {
//fmt.Println("----------------------------")
    err := filepath.Walk(dir, func(path string, info os.FileInfo, err error) error {
        //_ , file := filepath.Split(path)
	//fmt.Println("file: ",file)
	if ( info.IsDir() ) {
	    dirs++
	} else if (info.Mode()&os.ModeSymlink != 0) {
	    links++;
	} else if (info.Mode()&os.ModeDevice != 0) {
	    devices++;
	} else if (info.Mode()&os.ModeSocket != 0) {
            sockets++;
        } else {
            otherFiles++;
        }
        //fmt.Println("file info: ", info.Mode())

        return nil
    })
//fmt.Println("-----------------------------")
    if err != nil {
        return err
    }

    fmt.Println("+-----------------------+--------+")
    fmt.Println("|Path                   |", dir, "\t |")
    fmt.Println("+-----------------------+--------+")
    fmt.Println("| Directories           |", dirs, "\t |")
    fmt.Println("| Symbolic Links        |", links, "\t |")
    fmt.Println("| Devices               |", devices, "\t |")
    fmt.Println("| Sockets               |", sockets, "\t |")
    fmt.Println("| Other Files           |", otherFiles, "\t |")
    fmt.Println("+-----------------------|--------+")
	return nil
}

var dirs, links, devices, sockets, otherFiles int = 0,0,0,0,0

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}

	scanDir(os.Args[1])
}
