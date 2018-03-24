#include "pulseaudioserver.h"
#include "config.h"

/*
 * Before starting lirc service, /var/run/lirc/ directory must be created
 * systemctl start lirc.service
 * /etc/init.d/lirc start
 *
 * /etc/lirc/lircd.conf:
 *
2 Настройка lirc
Редактируем с привелегиями суперпользователя файл /etc/lirc/hardware.conf
(для этого можно например ввести в терминале: sudo gedit /etc/lirc/hardware.conf)
В самом файле устанавливаем соответствующие параметры приёмника:

REMOTE="Creative_RM820"
REMOTE_MODULES=""
REMOTE_DRIVER="alsa_usb"
REMOTE_DEVICE="hw:Pro"
REMOTE_SOCKET=""
REMOTE_LIRCD_CONF="creative/lircd.conf.alsa_usb"
REMOTE_LIRCD_ARGS=""

а также
START_LIRCD="true"

после чего сохраняем файл и выходим из редактора.

Колёсико задействовано уже в alsa_usb, но чтобы работал пульт надо сделать следующее:
в папке /etc/lirc есть также файл lircd.conf, открываем его в редакторе с правами суперпользователя (sudo gedit /etc/lirc/lircd.conf) и пишем:

include "/usr/share/lirc/remotes/creative/lircd.conf.alsa_usb"

begin remote
  name            Creative_RM820
  bits            8
  eps            30
  aeps          100

  one             0     0
  zero            0     0
  gap          103993
  toggle_bit_mask 0x0

      begin codes
          KEY_DOWN                 0x32
          KEY_UP                   0x31
          KEY_LEFT                 0x27
          KEY_RIGHT                0x28
          KEY_OK                   0x30
          KEY_VOLUMEUP             0x2B
          KEY_VOLUMEDOWN           0x2C
          KEY_MUTE                 0x2A
          KEY_POWER                0x29
          KEY_MENU                 0x1B
          KEY_FASTFORWARD          0x36
          KEY_REWIND               0x35
          KEY_PLAYPAUSE            0x2F
          KEY_SHUFFLE              0x3B
          KEY_EXIT                 0x1A
          KEY_REDO                 0x3C
          KEY_NEXT                 0x34
          KEY_PREV                 0x33
      end codes
end remote

сохраняем этот файл и выходим из редактора.

Теперь надо перезапустить lirc, для этого в терминале вводим:
sudo /etc/init.d/lirc restart

then create in home directory .lircrc file and put this:
begin
    remote = Creative_RM820
    button = KEY_VOLUMEUP
    prog = irexec
    config = /home/cross/workspace/pasoundctrl/Debug/pasoundctrl inc 5
    repeat = 1
end
 */

int main(int argc, char ** argv)
{
	if (argc < 2)
	{
		std::cout << "Usage :" << argv[0] << " inc|dec val" << std::endl;
		return 0;
	}

	std::string command = argv[1];
	int val = 0;

	if (argc == 3)
	{
		std::string value = argv[2];

		std::string::size_type sz;
		val = std::stoi(value, &sz);
	}

	try
	{
		pulse_audio_server pa_server;
		refcount_ptr<pulse_audio_device_info> pulseAudioDeviceInfo = pa_server.get_audio_device_info(config::devicename());

		if (command.compare("inc") == 0)
		{
			pulseAudioDeviceInfo->volume_increment(val);
		}
		else if (command.compare("dec") == 0)
		{
			pulseAudioDeviceInfo->volume_decrement(val);
		}
		else if (command.compare("mute") == 0)
		{
			pulseAudioDeviceInfo->muted(!pulseAudioDeviceInfo->muted());
		}
		else if (command.compare("power") == 0)
		{
			sync();
			reboot(RB_POWER_OFF);
		}

		pa_server.set_audio_device_info(pulseAudioDeviceInfo);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}


    return 0;
}
