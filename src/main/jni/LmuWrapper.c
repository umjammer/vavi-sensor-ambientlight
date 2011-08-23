/* motion.c
 *
 * a little program to display the coords returned by
 * the powerbook motion sensor
 *
 * A fine piece of c0de, brought to you by
 *
 *               ---===---
 * *** teenage mutant ninja hero coders ***
 *               ---===---
 *
 * All of the software included is copyrighted by Christian Klein <chris@5711.org>.
 *
 * Copyright 2005 Christian Klein. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Christian Klein.
 * 4. The name of the author must not be used to endorse or promote
 *    products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Modified for iBook compatibility by Pall Thayer <http://www.this.is/pallit>
 */

#include "vavi_sensor_ambientlight_macbook_MacbookAmbientLight.h"

#include <stdio.h>
#include <string.h>
#include <IOKit/IOKitLib.h>

enum {
	kGetSensorReadingID = 0, // getSensorReading(int *, int *)
	kGetLEDBrightnessID = 1, // getLEDBrightness(int, int *)
	kSetLEDBrightnessID = 2, // setLEDBrightness(int, int, int *)
	kSetLEDFadeID = 3, // setLEDFade(int, int, int, int *)
	// other firmware-related functions
	// verifyFirmwareID = 4, // verifyFirmware(int *)
	// getFirmwareVersionID = 5, // getFirmwareVersion(int *)
	// other flashing-related functions  // ...
};

static kern_return_t result;
static mach_port_t masterPort;
static io_iterator_t iterator;
static io_object_t aDevice;
static io_connect_t  dataPort;

#ifdef __LP64__
static uint64_t outputData[2];
static uint32_t outputCount;
#else
SInt32 left, right;
#endif

/*
 * Class:     vavi_sensor_ambientlight_macbook_MacbookAmbientLight
 * Method:    init
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_vavi_sensor_ambientlight_macbook_MacbookAmbientLight_init
(JNIEnv *env, jobject obj) {

	result = IOMasterPort(MACH_PORT_NULL, &masterPort);

	CFMutableDictionaryRef matchingDictionary = IOServiceMatching("AppleLMUController");

	result = IOServiceGetMatchingServices(masterPort, matchingDictionary, &iterator);

	if (result != KERN_SUCCESS)	{
		return -1;
	}

	aDevice = IOIteratorNext(iterator);
	IOObjectRelease(iterator);

	if(aDevice == 0) {
		return -2;
	}

	result = IOServiceOpen(aDevice, mach_task_self(), 0, &dataPort);
	IOObjectRelease(aDevice);

	if (result != KERN_SUCCESS) {
		return -3;
	}

#ifdef __LP64__
    result = IOConnectCallScalarMethod(
		dataPort,
		kGetSensorReadingID,
		NULL,
		0,
		outputData,
		&outputCount
	);
fprintf(stderr, "result: %x, %d\n", (unsigned int) result, (int) outputCount);
fflush(stderr);
#else
	result = IOConnectMethodScalarIScalarO(
		dataPort,
		kGetSensorReadingID,
		0, 
		2,
		&left,
		&right
	);
#endif

	if (result != KERN_SUCCESS) {
		return -4;
	}

	return 0;
}

/*
 * Class:     vavi_sensor_ambientlight_macbook_MacbookAmbientLight
 * Method:    sence
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_vavi_sensor_ambientlight_macbook_MacbookAmbientLight_sense
(JNIEnv *env, jobject obj) {

#ifdef __LP64__
    result = IOConnectCallScalarMethod(
		dataPort,
		kGetSensorReadingID,
		NULL,
		0,
		outputData,
		&outputCount
	);
#else
	result = IOConnectMethodScalarIScalarO(
		dataPort,
        kGetSensorReadingID,
		0, 
		2,
		&left,
		&right
	);
#endif
	
	if (result != KERN_SUCCESS) {
		return -4;
	}
	
	return 0;
}

/*
 * Class:     vavi_sensor_ambientlight_macbook_MacbookAmbientLight
 * Method:    destroy
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_vavi_sensor_ambientlight_macbook_MacbookAmbientLight_destroy
(JNIEnv *env, jobject obj) {

	IOServiceClose(dataPort);
}


/*
 * Class:     vavi_sensor_ambientlight_macbook_MacbookAmbientLight
 * Method:    getLeft
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_vavi_sensor_ambientlight_macbook_MacbookAmbientLight_getLeft
(JNIEnv *env, jobject obj) {

#ifdef __LP64__
	return (jint) outputData[0];
#else
	return (jint) left;
#endif
}

/*
 * Class:     vavi_sensor_ambientlight_macbook_MacbookAmbientLight
 * Method:    getRight
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_vavi_sensor_ambientlight_macbook_MacbookAmbientLight_getRight
(JNIEnv *env, jobject obj) {

#ifdef __LP64__
	return (jint) outputData[1];
#else
	return (jint) right;
#endif
}
