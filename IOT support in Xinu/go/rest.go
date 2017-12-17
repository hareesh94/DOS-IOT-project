package main

import (
	"fmt"
	"log"
	"net/http"
	"net"
	"strconv"
	"strings"
	"bufio"
)

type query struct {
	Glowled int
	Status bool
}

func parseBool(s string, dest *bool) error {
	// assume error = false
	*dest, _ = strconv.ParseBool(s)
	return nil
}

func communicate ( led int , status bool) string {
	p :=  make([]byte, 2048)
	cmd := 0
	ans := "NULL"
	conn, err := net.Dial("udp", "192.168.11.111:9")
	if err != nil {
		fmt.Printf("Some error %v", err)
		return ans
	}

	if  status == true {
		cmd =1
	} else {
		cmd =0
	}	
	msg :=  fmt.Sprintf("%d:%d",led,cmd)
	fmt.Fprintf(conn, msg)
	fmt.Printf("data sent %s",msg)
	_, err = bufio.NewReader(conn).Read(p)
	ack := fmt.Sprintf("%s",p)
	if err == nil {
		if strings.Contains(ack,msg) {
			fmt.Printf("ACK received %s\n", p)
			s := strings.Split(ack, ":")
			conn.Close()
			fmt.Printf("Values are %s:%s:%s\n",s[0],s[1],s[2])
			return s[2]
		} else {
			fmt.Printf("Corrupt Ack\n");
		}
	} else {
		fmt.Printf("Some error %v\n", err)
	}
	conn.Close()
	return ans

}

func parseInt(s string, dest *int) error {
	n, err := strconv.Atoi(s)
	if err != nil {
		return err
	}
	*dest = n
	return nil
}

func handler(w http.ResponseWriter, req *http.Request) {
	if err := req.ParseForm(); err != nil {
		log.Printf("Error parsing form: %s", err)
		return
	}
	q := &query{}
	ans := "NULL"
	if err := parseBool(req.Form.Get("Status"), &q.Status); err != nil {
		log.Printf("Error parsing dryrun: %s", err)
		return
	}
	if err := parseInt(req.Form.Get("Glowled"), &q.Glowled); err != nil {
		log.Printf("Error parsing limit: %s", err)
		return
	}

	ans = communicate(q.Glowled,q.Status)

	fmt.Print(ans)
	fmt.Fprintf(w,ans)
}

func main() {
	log.Fatal(http.ListenAndServe(":8080", http.HandlerFunc(handler)))
}
