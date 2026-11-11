#病情诊断系统  202200130077 薛润泽
#自定义函数 判断具体病症
def f_illness(set1):
    print('具体病症如下')
    if 12 in set1:
        if 10 in set1:
            if 1 in set1:
                print('外科，肌肉酸痛，头痛->过劳')
            if 11 in set1:
                print('外科，肌肉酸痛，肌肉淤血发青->肌肉损伤')
        elif 11 in set1:
            print('外科，肌肉淤血发青->血管阻塞')
    if 13 in set1:
        if 1 in set1 and 6 in set1:
            print('神经科，头痛，失眠->神经衰弱')
        if 9 in set1 :
            print('神经科，精神低迷->心结')
    if 14 in set1:
        if 5  in set1:
            print('呼吸内科,头痛，咳嗽,胸闷->流感')
        if 7 in set1:
            print('呼吸内科，腹泻->流感')
        if 3 in set1:
            print('呼吸内科，流涕->风寒')
    if 15 in set1:
        if 7 and 8 in set1:
            print('消化科，呕吐，腹泻->食物中毒')
        elif 7  in set1:
            print('消化科，腹泻->食物中毒')
        elif 8 in set1:
            print('消化科，呕吐->食物中毒')
    return 0
#字典
dictionary={1:'头痛',2:'发热',3:'流涕',4:'咳嗽',5:'胸闷',6:'失眠',7:'腹泻',8:'呕吐',9:'精神低迷',10:'肌肉酸痛',\
11:'肌肉淤血发青',12:'外科',13:'神经科',14:'呼吸内科',15:'消化科',16:'流感',17:'伤寒',18:'神经衰弱',19:'肌肉损伤',\
20:'过劳',21:'食物中毒',22:'血管阻塞'}

print('''输入对应条件前面的数字:
                                *******************************************************
                                *1:头痛  2:发热  3:流涕  4:咳嗽      5:胸闷           *
                                *6:失眠  7:腹泻  8:呕吐  9:精神低迷  10:肌肉酸痛      *
                                *11: 肌肉淤血发青
                                *******************************************************
                                *******************当输入数字0时!程序结束***************
     ''' )
#综合数据库
setin1=set()
while True:
    receive =int(input("请输入上述症状的序号"))
    if receive == 0:
        break;
    if receive>11 or receive <1:
        print('请输入正确序号')
    setin1.add(receive)
print()
print('前提条件为:')
for x in range(1,12):
    if x in setin1:
        print(dictionary[x],end='  ')


print()
print('推理过程如下：')

setchk2=0
if 2  in setin1:
    print('发热->呼吸内科')
    setin1.add(14)
    setchk2=1
if 4 in setin1:
    print('咳嗽->呼吸内科')
    setin1.add(14)
    setchk2=1
if 1 in setin1:
    if 6 in setin1:
            print('头痛，失眠—>神经科')
            setchk2=1
            setin1.add(13)
    if 9 in setin1:
            print('精神低迷->神经科')
            setin1.add(13)
            setchk2=1
if 10 in setin1 or  11 in setin1:
    print('肌肉酸痛—>外科')
    setin1.add(12)
    setchk2=1
if 8 in setin1:
    print('呕吐—>消化科')
    setchk2=1
    setin1.add(15)
if setchk2==0:
    print('没有检测出相应病症，请重新输入事实：')
else:
    f_illness(setin1)







        
