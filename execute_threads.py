import threading
import os
import subprocess
import sys

project_path = os.getcwd()
usb_port = ''

def roscore_thread():
	print(threading.currentThread().getName(), ' Starting Process for roscore')
	os.system('roscore')
	print(threading.currentThread().getName(), ' Roscore Finished Process')

def python_thread():
	print(threading.currentThread().getName(), ' Starting Process for publishing System Monitor')
	os.chdir(project_path)
	os.system('python system_manager.py')
	print(threading.currentThread().getName(), ' Publishing Process Finished Process')

def rosserial_thread():
	print(threading.currentThread().getName(), ' Starting Process')
	os.system('rosrun rosserial_python serial_node.py ' + usb_port)
	print(threading.currentThread().getName(), ' Finished Process')

def check_usb_port():
	print(threading.currentThread().getName(), ' Starting Process')
	usb = subprocess.Popen("ls /dev/ttyUSB* 2> /dev/null", stdout=subprocess.PIPE, shell=True)
	(output, error) = usb.communicate()
	if(output == '' or error != None):
		print("\n USB Connection Lost or Not Sycronized !")
		sys.exit('\n Connection not established...')
	else:
		usb_port = output
	print(threading.currentThread().getName(), ' Finished Process')

ls_process = threading.Thread(name='ls_service', target=check_usb_port)
roscore_process = threading.Thread(name='roscore_service', target=roscore_thread)
python_process = threading.Thread(name='python_service', target=python_thread)
rosserial_process = threading.Thread(name='rosserial_service', target=rosserial_thread)

def main():
	ls_process.start()
	if(usb_port == ''):
		roscore_process.start()
		python_process.start()
		rosserial_process.start()
	else:
		sys.exit()

if __name__ == "__main__":
	main()


