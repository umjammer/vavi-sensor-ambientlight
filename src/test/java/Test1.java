/*
 * Copyright (c) 2008 by Naohide Sano, All rights reserved.
 *
 * Programmed by Naohide Sano
 */

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.condition.EnabledIfSystemProperty;
import vavi.sensor.ambientlight.AmbientLight;
import vavi.sensor.ambientlight.macbook.MacbookAmbientLight;


/**
 * test01.
 * 
 * @author <a href="mailto:umjammer@gmail.com">Naohide Sano</a> (nsano)
 * @version 0.00 070202 initial version <br>
 */
public class Test1 {

    @Test
    @EnabledIfSystemProperty(named = "vavi.test", matches = "ide")
    public void test01() throws Exception {
        AmbientLight accelerometer = new MacbookAmbientLight();
        for (int i = 0; i < 1000; i++) {
            accelerometer.sense();
            System.err.printf("%08x, %08x\n", accelerometer.getLeft(), accelerometer.getRight());
            Thread.sleep(100);
        }
    }

    /** */
    public static void main(String[] args) throws Exception {
        Test1 test = new Test1();
        test.test01();
    }
}

/* */
