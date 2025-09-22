param(
    [string]$Ip = '127.0.0.1',
    [int]$Port = 41451
)

Write-Host "Тестуємо TCP підключення до $Ip:$Port ..."
$res = Test-NetConnection -ComputerName $Ip -Port $Port -WarningAction SilentlyContinue
if ($res.TcpTestSucceeded) {
    Write-Host "[OK] TCP порт відкрито" -ForegroundColor Green
} else {
    Write-Host "[FAIL] Порт недоступний" -ForegroundColor Red
    if ($res) { $res | Format-List | Out-String | Write-Host }
    exit 1
}

