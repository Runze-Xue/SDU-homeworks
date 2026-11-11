import subprocess 
import os
# dir = "D:/大学/大三上/认知科学与类脑识别/大作业"
# if os.path.exists(dir):
#     os.chdir(dir)
powerShellCommand =  'python GPT-SoVITS-beta0706/control.py'
result = subprocess.run(["powershell", "-Command", powerShellCommand], capture_output=False, text=True)