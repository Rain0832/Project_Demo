function getRosePrices() {
    const dateInput = document.getElementById('date');
    const loading = document.getElementById('loading');
    const result = document.getElementById('result');
    
    // 验证日期输入
    if (!dateInput.value) {
        alert('请选择日期！');
        return;
    }

    // 显示加载状态
    loading.style.display = 'block';
    result.innerHTML = '';

    // 模拟API请求
    setTimeout(() => {
        calculatePrices(dateInput.value).then(prices => {
            displayResults(prices);
            loading.style.display = 'none';
        });
    }, 1000);
}

async function calculatePrices(date) {
    try {
        const response = await fetch(`http://localhost:8080/api/prices?date=${date}`);
        if (!response.ok) {
            throw new Error('获取价格失败');
        }
        return await response.json();
    } catch (error) {
        alert('获取价格信息失败：' + error.message);
        return {
            single: 0,
            dozen: 0,
            bouquet: 0
        };
    }
}

function displayResults(prices) {
    const result = document.getElementById('result');
    result.innerHTML = `
        <div class="price-item">
            <h3>单支玫瑰</h3>
            <p>￥${prices.single}</p>
        </div>
        <div class="price-item">
            <h3>一打玫瑰（12支）</h3>
            <p>￥${prices.dozen}</p>
        </div>
        <div class="price-item">
            <h3>精美花束</h3>
            <p>￥${prices.bouquet}</p>
        </div>
    `;
}

// 添加新函数来获取可用日期
async function getAvailableDates() {
    try {
        const response = await fetch('http://localhost:8080/api/available-dates');
        if (!response.ok) {
            throw new Error('获取日期失败');
        }
        const dates = await response.json();
        displayAvailableDates(dates);
    } catch (error) {
        alert('获取可用日期失败：' + error.message);
    }
}

// 显示可用日期
function displayAvailableDates(dates) {
    const result = document.getElementById('available-dates');
    if (dates.length === 0) {
        result.innerHTML = '<p>暂无价格数据</p>';
        return;
    }

    const table = `
        <table>
            <thead>
                <tr>
                    <th>日期</th>
                    <th>基础价格</th>
                </tr>
            </thead>
            <tbody>
                ${dates.map(date => `
                    <tr>
                        <td>${date.date}</td>
                        <td>￥${date.price}</td>
                    </tr>
                `).join('')}
            </tbody>
        </table>
    `;
    result.innerHTML = table;
}

// 页面加载时获取可用日期
document.addEventListener('DOMContentLoaded', () => {
    const dateInput = document.getElementById('date');
    const today = new Date().toISOString().split('T')[0];
    dateInput.min = today;
    
    // 获取可用日期
    getAvailableDates();
});
