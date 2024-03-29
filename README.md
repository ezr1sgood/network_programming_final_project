# 台灣政治大富翁 (Taiwan Political Monopoly)
![Alt text](Title.png)
## 遊戲簡介
在這個虛擬的台灣政治世界中，玩家們將扮演自己創立的政治角色，踏上一條充滿策略與挑戰的財富之路。遊戲融合了大富翁的經典玩法，並以台灣特有的政治議題和文化為背景，增添了許多獨特的遊戲元素。



## 遊戲特色：

- 政黨機制： 玩家可以選擇加入不同的政黨，並不斷地在政黨提升自身地位，最終成為總統贏得遊戲勝利！透過策略組隊和合作，玩家可以更有效地獲得資源和影響力。

- 血量機制： 除了財富，玩家還有一個血量指標。在遊戲中，玩家必須小心管理自己的血量，以避免因政治鬥爭或不良決策而被淘汰。

- 技能機制： 玩家可以透過獲得技能卡來增強自己的能力，例如增加投資回報、減少罰款或影響對手的策略。

- 諷刺與挑戰： 遊戲中充滿了對台灣政治的諷刺和反思，從外役監到官商勾結，每個元素都反映了台灣政治的現實面貌。

- 策略與競爭： 玩家需要善用資源、策略和運氣，與其他玩家競爭，最終成為台灣政治界的大富翁。

## 遊戲要素及玩法
- 玩家需要和大富翁的 server 進行連線，連線成功後即可進入遊戲
  - 若是第一位玩家，可決定玩家的人數
  - 其餘玩家須等待房間其他人
  - 房間人數到達第一位玩家指定的即可開始遊戲

- 遊戲中...
  - 在玩家回合時，在 console 中輸入字串作為隨機種子進行擲骰子的動作
  - 踩到房地產上時，可購買此地或者是要支付過路費給擁有者
  - 若到達機會命運，將會觸發隨機事件或是獲得技能卡，隨機事件包含各種台灣文化(小心被三寶撞)
  - 每次經過起點可以獲得 2000 元
  - 經過監獄需要休息兩回合
  - 死亡、負債的玩家會被永遠停止行動，其他玩家繼續進行遊戲

## 機會卡
我們的機會卡牌與傳統的大富翁僅僅是抽卡，並且損失金錢、賺取金錢或是進監獄等等的事件不同，我們的機會卡牌是玩家未來可以使用的技能。
- 機會卡牌種類
  - **外役監啦，哈哈**：
    - 限制：你必須已經加入一個政黨才能使用此卡片。
    - 效果：如果當前所在格子為監獄，則可立即離開監獄，並且獲得獄中人脈，政黨階級+1。
  - **政治鬥爭**
    - 效果：指定一個已經有政黨的玩家，使其政黨階級 -1。
  - **神風特攻隊**
    - 效果：攻擊其中一個格子，建築物夷平玩家生命值減少 50。
  - **機車三寶**
    - 效果：指定一位玩家，令該玩家生命值減少 20。

## 命運卡
命運卡與傳統大富翁較為類似，是一系列對玩家造成正面或負面影響的事件。
- 命運卡牌種類
  - **快步過馬路**
    - 說明：因為你在台灣快步走過馬路，導致視野死角，被機車撞到。
    - 效果：生命值減少 20。
  - **慢步過馬路**
    - 說明：因為你在台灣慢步走過馬路，引起大眾觀感不佳。
    - 效果：如果你有政黨，政黨階級 -1。
  - **背鍋俠**
    - 說明：你是教育部的臉書粉專小編，教育部亂拍影片遭到公眾撻伐，你被迫『具名』背鍋，身心受創。
    效果：生命值減少 50。[新聞連結](https://fongnews.net/political/83116/)
  - **大撒幣**
    - 說明：總統大選即將到來，政府決定補助私立大學學費 3.5 萬元，但花的不是他的錢，是你這個納稅人的錢。
    - 效果：你的金錢減少 500。
  - **中樂透**
    - 說明：在小七無聊買個可爾必思沒事就中了 1000 萬元發票。
    - 效果：你的金錢增加 5000。

## Server-Client 流程圖
![Alt text](flowChart_1.png)

![Alt text](flowChart_2.png)

## 勝敗判定 (Class Game)
- 政黨: 踩到指定的政黨格，並支付政治獻金提升黨階，在同一個政黨累積三次成為總統後遊戲直接獲勝
- 血條: 所有玩家初始血量100，血條歸 0 則判定輸
- 金錢: 金錢歸 0 則判定輸

```

                                                                                        
          ____                                                                          
        ,'  , `.                                                      ,--,              
     ,-+-,.' _ |                                ,-.----.            ,--.'|              
  ,-+-. ;   , ||   ,---.        ,---,    ,---.  \    /  \    ,---.  |  | :              
 ,--.'|'   |  ;|  '   ,'\   ,-+-. /  |  '   ,'\ |   :    |  '   ,'\ :  : '              
|   |  ,', |  ': /   /   | ,--.'|'   | /   /   ||   | .\ : /   /   ||  ' |        .--,  
|   | /  | |  ||.   ; ,. :|   |  ,"' |.   ; ,. :.   : |: |.   ; ,. :'  | |      /_ ./|  
'   | :  | :  |,'   | |: :|   | /  | |'   | |: :|   |  \ :'   | |: :|  | :   , ' , ' :  
;   . |  ; |--' '   | .; :|   | |  | |'   | .; :|   : .  |'   | .; :'  : |__/___/ \: |  
|   : |  | ,    |   :    ||   | |  |/ |   :    |:     |`-'|   :    ||  | '.'|.  \  ' |  
|   : '  |/      \   \  / |   | |--'   \   \  / :   : :    \   \  / ;  :    ; \  ;   :  
;   | |`-'        `----'  |   |/        `----'  |   | :     `----'  |  ,   /   \  \  ;  
|   ;/                    '---'                 `---'.|              ---`-'     :  \  \ 
'---'                                             `---`                          \  ' ; 
                                                                                  `--`  

```
