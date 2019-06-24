import psutil as monitor
import rospy
from std_msgs.msg import String


def get_system_usage():
    system_usages = [0.0, 0.0, 0.0, 0.0, 0.0]
    system_usages[0] = monitor.cpu_percent(interval = 1)
    system_usages[1] = monitor.virtual_memory()[2]
    system_usages[2] = monitor.disk_usage('/')[3]
    system_usages[3] = "%.1f" % monitor.sensors_battery().percent
    system_usages[4] = monitor.sensors_temperatures()["acpitz"][0].current
    return system_usages

def get_msg():
    system_usage = get_system_usage()
    message = str(system_usage[0]) + "/" + str(system_usage[1]) + "/" + str(system_usage[2]) + "/" + str(system_usage[3]) + "/" + str(system_usage[4])
    return message

if __name__ == '__main__':
    rospy.init_node('system_monitor_publisher')
    monitor_publisher = rospy.Publisher('system_monitor', String, queue_size=10)
    rate = rospy.Rate(100)
    msg_str = String
    while not rospy.is_shutdown():
        msg_str = get_msg()
        monitor_publisher.publish(msg_str)
        rate.sleep()
