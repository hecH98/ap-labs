package main

 import (
 	"golang.org/x/tour/wc"
 	"strings"
 )

 func WordCount(s string) map[string]int {
 	array := strings.Fields(s)
 	map2 := make(map[string]int)
 	for _,v := range array {
 		map2[v]++
 	}
 	return map2
 }

 func main() {
 	wc.Test(WordCount)
 }
