/*
 * Copyright (c) 2009 by Naohide Sano, All rights reserved.
 *
 * Programmed by Naohide Sano
 */

package vavi.sensor.ambientlight.macbook;

import vavi.sensor.ambientlight.AmbientLight;


/**
 * MacbookAmbientLight. 
 *
 * @author <a href="mailto:vavivavi@yahoo.co.jp">Naohide Sano</a> (nsano)
 * @version 0.00 2009/08/24 nsano initial version <br>
 */
public class MacbookAmbientLight implements AmbientLight {

    /** */
    private native int init();
    
    /** */
    public native int sense();

    /** */
    private native void destroy();
    
    public MacbookAmbientLight() {
        int r = init();
        switch (r) {
        case -1:
            throw new IllegalStateException("IOServiceGetMatchingServices returned error.");
        case -2:
            throw new IllegalStateException("No motion sensor available");
        case -3:
            throw new IllegalStateException("Could not open motion sensor device");
        case -4:
            throw new IllegalStateException("no coords");
        }
    }

    protected void finalize() throws Throwable {
        destroy();
    }

    public native int getLeft();

    public native int getRight();

    static {
        try {
            System.loadLibrary("LmuWrapper");
        } catch (Exception e) {
            throw new IllegalStateException(e);
        }
    }
}

/* */
