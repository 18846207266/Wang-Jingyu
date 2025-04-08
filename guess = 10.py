guess = 10
if int(input("请猜一个数字：")) == guess :
    print("只猜一次就对了！")
elif int(input("猜第二次:")) == guess :
    print("第二次猜对咯~")
elif int(input("猜第三次:")) == guess :
    print("恭喜你捏，最后一次机会猜对咯~")
else :
    print("倒霉蛋你没猜对")