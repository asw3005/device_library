public static class Crc8 {
    static byte[] table = new byte[256];
    // x8 + x7 + x6 + x4 + x2 + 1
    const byte poly = 0xd5;

    public static byte ComputeChecksum(params byte[] bytes ) {
        byte crc = 0;
        if ( bytes != null && bytes.Length > 0 ) {
            foreach ( byte b in bytes ) {
                crc = table[crc ^ b];
            }
        }
        return crc;
    } 

    static Crc8( ) {
        for ( int i = 0; i < 256; ++i ) {
            int temp = i;
            for ( int j = 0; j < 8; ++j ) {
                if ( ( temp & 0x80 ) != 0 ) {
                    temp = ( temp << 1 ) ^ poly;
                } else {
                    temp <<= 1;
                }
            }
            table[i] = (byte)temp;
        }
    }
}


byte crc = Crc8.ComputeChecksum( 1, 2, 3 );
byte check = Crc8.ComputeChecksum( 1, 2, 3, crc );
// here check should equal 0 to show that the checksum is accurate
if ( check != 0 ) {
    Console.WriteLine( "Error in the checksum" );
}