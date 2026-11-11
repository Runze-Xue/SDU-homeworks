from matplotlib import pyplot as plt                                  # 展示图片
import numpy as np                                               # 数值处理
import cv2                                                       # opencv库
from sklearn.linear_model import LinearRegression, Ridge, Lasso          # 回归分析

def load_image(img_path):     #img_path:图像路径，一般为相对路径(将数据放于代码文件所在文件夹下)
    ''' 加载图片 '''

    # 通过opencv库，使用imread()方法将图片读取为BGR数值矩阵
    img = cv2.imread(img_path)
    
    # 如果读入的图像为BGR数值矩阵，将其转换为RGB图像
    if len(img.shape) == 3:
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB) #参数cv2.COLOR_BGR2RGB表示图像由BGR转换为RGB
    
    # 为了方便后续随机噪声mask图像后用于区别mask区域（像素值都将变为0），将原始图像=0的值变为0.01（PS:一般图片不会出现数值0）
    img[(img == 0)] = 0.01
        
    return img

def add_noise(img, noise_ratio):   #img：处理的图像对象；noise_ratio:噪声
    ''' 添加噪声 '''
    
    # 采用二值伯努利分布按比例采样mask噪声
    #numpy.random.binomial(n,p,size=None)，n:一次实验的样本数，此次实验针对一张图片；以(1 - noise_ratio)的概率获取像素，最终以图片的尺寸返回结果
    noise_img = np.random.binomial(1, 1 - noise_ratio, size=img.shape) 
    
    # 矩阵乘法，用噪声mask掉图像上的对应元素
    noise_img = np.multiply(noise_img,img)

    return noise_img

def normalization(img):
    ''' 最大值归一化 '''
    return img / np.max(img)  #图像中的所有像素点除以其中的最大值，做归一化处理

def restore(img, domain_length):  #参数img：研究的图像对象； domain_length:像素点周围的区域长度
    ''' 
    区域二元线性回归模型 -- 图像恢复 
    :param domain_length: 区域大小
    :return: 恢复过后的图像矩阵
    '''
    
    # 深拷贝图像矩阵
    resImg = np.copy(img)
    
    # 区别噪声mask
    noiseMask = np.array(img != 0, dtype='double')
    
    # 获取图像矩阵大小的各个维度值，行，列，通道
    rows, cols, channel = img.shape
    
    count = 0   #控制拟合的次数
    for row in range(rows):
        for col in range(cols):
            for chan in range(channel):
                # 如果该像素识别未添加过噪声，就不对其进行恢复
                if noiseMask[row, col, chan] != 0.:
                    continue
                    
                x_train = []
                y_train = []
                for i in range(row-domain_length, row+domain_length):
                    # 边界条件，超出图像行范围的不考虑
                    if i < 0 or i >= row:
                        continue
                    for j in range(col-domain_length, col+domain_length):
                        # 边界条件，超出图像列范围的不考虑
                        if j < 0 or j >= col:
                            continue
                        # 被噪声mask掉的区域不参与真实图像回归
                        elif noiseMask[i, j, chan] == 0.:
                            continue
                        # 自己不作为真实图像回归的一部分
                        elif i == row and j == col:
                            continue
                        # 其余符合条件的区域内的像素点作为二元拟合的样本
                        x_train.append([i, j])
                        y_train.append([img[i, j, chan]])
                        
                #如果不存在可拟合的样本，就跳过
                if x_train == []:
                    continue
                
                # 区域二元线性拟合
                Regression = LinearRegression()    #导入线性回归函数
                Regression.fit(x_train, y_train)   #拟合训练集数据
                resImg[row, col, chan] = Regression.predict([[row, col]])  #使用模型预测点(row, col, chan)的原始像素值
            
            # 每5000次拟合进行一次输出
            count += 1
            if count % 5000 == 0:
                print("picture restored:" +
                      str(float(count)/rows/cols))
                
    print("picture restore finish!")
    return resImg

def loss(img_1, img_2):
    ''' 采用2-范数进行loss计算 '''
    
    loss_ = np.sqrt(np.sum(np.power(img_1 - img_2, 2)))
    return loss_

def save_image(img_, img_path):        #img_path:保存到本地的文件夹路径
    ''' 采用opencv库将图像保存到本地'''

    img = np.copy(img_)     #深拷贝图像
    
    # 从给定数组的形状中删除维度为1的条目
    img = img.squeeze()
        # 将图片数据存储类型改为 np.uint8
    if img.dtype == np.double:
        
        #np.iinfo(np.unit8).max找到unit8数据类型中的最大值。这边的操作是否与数据最大值归一化操作有不对应的地方？？？
        img = img * np.iinfo(np.uint8).max   
        
        # 转换图片数组数据类型
        img = img.astype(np.uint8)
    
    # 将 RGB 方式转换为 BGR 方式
    img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)
    # 生成图片
    cv2.imwrite(img_path, img)

# 图片路径
img_path = r'D:\360Downloads\AItest\testpicture.jpg'
# 加载图片
img = load_image(img_path)
img = normalization(img)      #归一化图片
# 展示图片
plt.title('testpicture')
plt.axis('off')
plt.imshow(img)

noise_img = add_noise(img, 0.3)  #img：原图； 噪声比例：0.3
# 展示噪声图片
plt.title('resultpicture')
plt.axis('off')
plt.imshow(noise_img)
# 保存噪声图片
save_image(noise_img,r'D:\360Downloads\AItest\resultpicture.jpg') #在data文件夹下面
# 计算距离
print("The loss is: ", loss(img, noise_img))

res_img = restore(noise_img, 3)  #将噪声图片作为输入，domain_length:3
# 展示恢复图片
plt.title('restorepitcture')
plt.axis('off')
plt.imshow(res_img)
# 保存恢复图像
save_image(res_img,r'D:\360Downloads\AItest\resotrepicture.jpg') #在data文件夹下
# 图片路径
img_path = r'D:\360Downloads\AItest\resotrepicture.jpg'
# 加载图片
img = load_image(img_path)
img = normalization(img)
# 展示图片	
plt.title('restorepicture.jpg')
plt.axis('off')
plt.imshow(img)
