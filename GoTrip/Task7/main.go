package main

import (
	"log"
	"net/http"
	"os"
	"os/signal"
	"syscall"
)

func main() {
	cfg, err := LoadConfig("config.json")
	if err != nil {
		log.Fatalf("Load config failed: %v", err)
	}

	storage, err := NewFileUserStorage(cfg.StoragePath)
	if err != nil {
		log.Fatalf("Failed to initialize storage: %v", err)
	}
	defer storage.Close()

	handler := NewUserHandler(storage)

	http.HandleFunc("/users", func(w http.ResponseWriter, r *http.Request) {
		switch r.Method {
		case http.MethodGet:
			handler.GetUsers(w, r)
		case http.MethodPost:
			handler.CreateUser(w, r)
		default:
			WriteError(w, &AppError{
				Code:    http.StatusMethodNotAllowed,
				Message: "Not support this method",
			})
		}
	})

	srv := &http.Server{
		Addr:    cfg.ServerPort,
		Handler: Logger(http.DefaultServeMux),
	}

	quit := make(chan os.Signal, 1)
	signal.Notify(quit, syscall.SIGINT, syscall.SIGTERM)

	go func() {
		<-quit
		log.Println("Receive quit signal, start free resource...")
		if err := srv.Close(); err != nil {
			log.Printf("server close err: %v", err)
		}
	}()

	log.Println("Server start in:", srv.Addr)
	if err := srv.ListenAndServe(); err != nil && err != http.ErrServerClosed {
		log.Fatalf("Server start failed: %v", err)
	}

	log.Println("Server quit elegantly")
}
