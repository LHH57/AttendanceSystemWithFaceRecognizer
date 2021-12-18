import os
import cv2


def change_dirs(path):
    files = os.listdir(os.path.join(os.getcwd(), path))
    name = '0'
    for file in files:
        if name != file.split('.')[0][-2:].lstrip('0'):
            name = file.split('.')[0][-2:].lstrip('0')
            os.mkdir(os.path.join(os.getcwd(), name))
            cnt = 0
        os.rename(src=os.path.join(os.getcwd(), path, file),
                  dst=os.path.join(os.getcwd(), name, str(cnt) + ".pgm"))
        cnt += 1


def look_img_size(path):
    img = cv2.imread(os.path.join(os.getcwd(), path))
    print(img.shape)
    cv2.imshow("look", img)
    cv2.waitKey(0)


def change_pic_format(folder_name, pic_nums=10):
    for cnt in range(1, pic_nums + 1):
        img = cv2.imread(os.path.join(os.getcwd(), folder_name, str(cnt) + '.jpg'))
        img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        img = cv2.resize(img, (231, 195), interpolation=cv2.INTER_CUBIC)
        cv2.imwrite(os.path.join(os.getcwd(), folder_name, str(cnt) + '.pgm'), img)


def remove_old_files(path, old_format: str):
    files = os.listdir(os.path.join(os.getcwd(), path))
    for file in files:
        if os.path.splitext(file)[1] == old_format:
            os.remove(os.path.join(os.getcwd(), path, file))


if __name__ == '__main__':
    pass
