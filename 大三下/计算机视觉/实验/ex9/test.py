import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-i1', '--img1','--img3', type=str)
parser.add_argument('-i2', '--img2', type=str)

args = parser.parse_args()

print(args.img1)
