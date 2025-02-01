package main

import (
	"database/sql"
	"encoding/json"
	"log"
	"net/http"
	"time"

	_ "github.com/go-sql-driver/mysql"
)

// PriceResponse 定义价格响应结构
type PriceResponse struct {
	Single  float64 `json:"single"`
	Dozen   float64 `json:"dozen"`
	Bouquet float64 `json:"bouquet"`
}

// 在 PriceResponse 结构体后添加新的结构体
type DatePrice struct {
	Date  string  `json:"date"`
	Price float64 `json:"price"`
}

// 定义基础价格常量
const (
	BasePrice           = 15.0
	ValentineMultiplier = 2.33
)

var db *sql.DB

func main() {
	// 初始化数据库连接
	var err error
	db, err = sql.Open("mysql", "root:123456@tcp(localhost:3306)/rose_shop?parseTime=true")
	if err != nil {
		log.Fatal("数据库连接失败:", err)
	}
	defer db.Close()

	// 测试数据库连接
	err = db.Ping()
	if err != nil {
		log.Fatal("数据库连接测试失败:", err)
	}
	log.Println("数据库连接成功")

	// 设置路由
	http.HandleFunc("/api/prices", enableCORS(handleGetPrices))
	http.HandleFunc("/api/available-dates", enableCORS(handleGetAvailableDates))

	// 设置静态文件服务
	fs := http.FileServer(http.Dir("."))
	http.Handle("/", fs)

	// 启动服务器
	log.Println("服务器启动在 http://localhost:8080")
	if err := http.ListenAndServe(":8080", nil); err != nil {
		log.Fatal("服务器启动失败:", err)
	}
}

// CORS 中间件
func enableCORS(handler http.HandlerFunc) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Access-Control-Allow-Origin", "*")
		w.Header().Set("Access-Control-Allow-Methods", "GET, OPTIONS")
		w.Header().Set("Access-Control-Allow-Headers", "Content-Type")

		if r.Method == "OPTIONS" {
			w.WriteHeader(http.StatusOK)
			return
		}

		handler(w, r)
	}
}

// 处理价格查询请求
func handleGetPrices(w http.ResponseWriter, r *http.Request) {
	if r.Method != "GET" {
		http.Error(w, "只支持GET请求", http.StatusMethodNotAllowed)
		return
	}

	// 获取日期参数
	dateStr := r.URL.Query().Get("date")
	if dateStr == "" {
		http.Error(w, "缺少日期参数", http.StatusBadRequest)
		return
	}

	// 解析日期
	date, err := time.Parse("2006-01-02", dateStr)
	if err != nil {
		http.Error(w, "日期格式无效", http.StatusBadRequest)
		return
	}

	// 从数据库获取基础价格
	basePrice, err := getPriceFromDB(date)
	if err != nil {
		// 如果数据库中没有找到价格，使用默认价格计算
		prices := calculatePrices(date)
		sendJSONResponse(w, prices)
		return
	}

	// 使用数据库中的价格计算最终价格
	prices := calculatePricesWithBase(date, basePrice)
	sendJSONResponse(w, prices)
}

// 从数据库获取价格
func getPriceFromDB(date time.Time) (float64, error) {
	var price float64
	err := db.QueryRow("SELECT price FROM rose_prices WHERE date = ?", date.Format("2006-01-02")).Scan(&price)
	if err == sql.ErrNoRows {
		return 0, err
	}
	if err != nil {
		log.Printf("数据库查询错误: %v", err)
		return 0, err
	}
	return price, nil
}

// 使用数据库价格计算最终价格
func calculatePricesWithBase(date time.Time, basePrice float64) PriceResponse {
	// 判断是否是情人节
	isValentinesDay := date.Month() == time.February && date.Day() == 14

	// 基础价格
	single := basePrice
	dozen := basePrice * 12 * 0.9 // 打折
	bouquet := basePrice * 15     // 花束包含更多装饰

	// 情人节价格调整
	if isValentinesDay {
		single *= ValentineMultiplier
		dozen *= ValentineMultiplier
		bouquet *= ValentineMultiplier
	}

	// 周末价格上浮10%
	if date.Weekday() == time.Saturday || date.Weekday() == time.Sunday {
		single *= 1.1
		dozen *= 1.1
		bouquet *= 1.1
	}

	return PriceResponse{
		Single:  float64(int(single*100)) / 100, // 保留两位小数
		Dozen:   float64(int(dozen*100)) / 100,
		Bouquet: float64(int(bouquet*100)) / 100,
	}
}

// 发送JSON响应的辅助函数
func sendJSONResponse(w http.ResponseWriter, data interface{}) {
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(data)
}

// 保留原有的calculatePrices函数作为后备计算方法
func calculatePrices(date time.Time) PriceResponse {
	// 判断是否是情人节
	isValentinesDay := date.Month() == time.February && date.Day() == 14

	// 基础价格
	single := BasePrice
	dozen := BasePrice * 12 * 0.9 // 打折
	bouquet := BasePrice * 15     // 花束包含更多装饰

	// 情人节价格调整
	if isValentinesDay {
		single *= ValentineMultiplier
		dozen *= ValentineMultiplier
		bouquet *= ValentineMultiplier
	}

	// 周末价格上浮10%
	if date.Weekday() == time.Saturday || date.Weekday() == time.Sunday {
		single *= 1.1
		dozen *= 1.1
		bouquet *= 1.1
	}

	return PriceResponse{
		Single:  float64(int(single*100)) / 100, // 保留两位小数
		Dozen:   float64(int(dozen*100)) / 100,
		Bouquet: float64(int(bouquet*100)) / 100,
	}
}

// 添加新的处理函数
func handleGetAvailableDates(w http.ResponseWriter, r *http.Request) {
	if r.Method != "GET" {
		http.Error(w, "只支持GET请求", http.StatusMethodNotAllowed)
		return
	}

	// 查询所有可用日期和价格
	rows, err := db.Query("SELECT date, price FROM rose_prices ORDER BY date")
	if err != nil {
		log.Printf("查询数据库错误: %v", err)
		http.Error(w, "服务器内部错误", http.StatusInternalServerError)
		return
	}
	defer rows.Close()

	var prices []DatePrice
	for rows.Next() {
		var dp DatePrice
		var date time.Time
		err := rows.Scan(&date, &dp.Price)
		if err != nil {
			log.Printf("读取数据错误: %v", err)
			continue
		}
		dp.Date = date.Format("2006-01-02")
		prices = append(prices, dp)
	}

	sendJSONResponse(w, prices)
}
