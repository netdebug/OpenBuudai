////////////////////////////////////////////////////////////////////////////////
//
//  OpenBuudai
/// \file buudai/device.h
/// \brief Declares the Buudai::Device class.
//
//  Copyright (C) 2008, 2009  Oleg Khudyakov
//  prcoder@potrebitel.ru
//  Copyright (C) 2010  Oliver Haag
//  oliver.haag@gmail.com
//
//  This program is free software: you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation, either version 3 of the License, or (at your option)
//  any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along with
//  this program.  If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef BUUDAI_DEVICE_H
#define BUUDAI_DEVICE_H


#include <QObject>
#include <QStringList>

#if LIBUSB_VERSION == 0
#include <usb.h>
#define libusb_device_handle usb_device_handle
#define libusb_device_descriptor usb_device_descriptor
#else
#include <libusb-1.0/libusb.h>
#endif


#include "helper.h"
#include "buudai/buudai_types.h"


namespace Buudai {
	//////////////////////////////////////////////////////////////////////////////
	/// \class Device                                              buudai/device.h
	/// \brief This class handles the USB communication with the oscilloscope.
	class Device : public QObject {
		Q_OBJECT
		
		public:
			Device(QObject *parent = 0);
			~Device();
			
			QString search();
			void disconnect();
			bool isConnected();
			
			// Various methods to handle USB transfers
#if LIBUSB_VERSION != 0
			int bulkTransfer(unsigned char endpoint, unsigned char *data, unsigned int length, int attempts = BUUDAI_ATTEMPTS_DEFAULT);
#endif
			int bulkWrite(unsigned char *data, unsigned int length, int attempts = BUUDAI_ATTEMPTS_DEFAULT);
			int bulkRead(unsigned char *data, unsigned int length, int attempts = BUUDAI_ATTEMPTS_DEFAULT);
			
            int bulkReadMulti(unsigned char *data, unsigned long int length, int attempts = BUUDAI_ATTEMPTS_DEFAULT);
			
			int controlTransfer(unsigned char type, unsigned char request, unsigned char *data, unsigned int length, int value, int index, int attempts = BUUDAI_ATTEMPTS_DEFAULT);
			int controlWrite(unsigned char request, unsigned char *data, unsigned int length, int value = 0, int index = 0, int attempts = BUUDAI_ATTEMPTS_DEFAULT);
			int controlRead(unsigned char request, unsigned char *data, unsigned int length, int value = 0, int index = 0, int attempts = BUUDAI_ATTEMPTS_DEFAULT);
			
			Model getModel();
		
		protected:
			// Lists for enums
			QList<unsigned short int> modelIds; ///< Product ID for each #Model
			QStringList modelStrings; ///< The name as QString for each #Model

			// Libusb specific variables
#if LIBUSB_VERSION != 0
			libusb_context *context; ///< The usb context used for this device
#endif
			Model model; ///< The model of the connected oscilloscope
#if LIBUSB_VERSION == 0
			usb_dev_handle *handle; ///< The USB handle for the oscilloscope
			usb_device_descriptor descriptor; ///< The device descriptor of the oscilloscope
#else
			libusb_device_handle *handle; ///< The USB handle for the oscilloscope
			libusb_device_descriptor descriptor; ///< The device descriptor of the oscilloscope
#endif
			int interface; ///< The number of the claimed interface
			int error; ///< The libusb error, that happened on initialization
			int outPacketLength; ///< Packet length for the OUT endpoint
			int inPacketLength; ///< Packet length for the IN endpoint
		
		signals:
			void connected(); ///< The device has been connected and initialized
			void disconnected(); ///< The device has been disconnected
			
		public slots:
			
	};
}


#endif
