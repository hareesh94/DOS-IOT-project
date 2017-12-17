package main
import (
    "fmt"
    "net"
    "bufio"
    "strings"
)

func main() {
    p :=  make([]byte, 2048)
    conn, err := net.Dial("udp", "192.168.11.111:9")
    if err != nil {
        fmt.Printf("Some error %v", err)
        return
    }
    msg := "123"
    fmt.Fprintf(conn, msg)
    _, err = bufio.NewReader(conn).Read(p)
    ack := fmt.Sprintf("%s",p)
    if err == nil {
	    if strings.Contains(ack,msg) {
        	fmt.Printf("ACK received %s\n", p)
	} else {
		fmt.Printf("Corrupt Ack\n");
	}
    } else {
        fmt.Printf("Some error %v\n", err)
    }
    conn.Close()
}
