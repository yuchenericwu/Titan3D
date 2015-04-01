using CppCliDll;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Titan3D
{
    public class BitmapWriter
    {
        private string filepath;
        private string base_filename;
        CppCliMatrix3D matrix;
        public readonly UInt32 width;
        private const UInt32 length = 300;
        public readonly UInt32 num_slices;
        public readonly UInt32 height;
        private const UInt32 x_multiple = 32;
        private const UInt32 z_scaling = 2;

        public BitmapWriter(string filepath, string base_filename,
                            CppCliMatrix3D matrix)
        {
            this.filepath = filepath;
            this.base_filename = base_filename;
            this.matrix = matrix;
            this.width = ((matrix.size_x() - 1) / x_multiple + 1) * x_multiple;
            this.num_slices = (matrix.size_y() - 1) / length + 1;
            this.height = matrix.size_z() / z_scaling;
        }

        public void Write(UpdateProgressCallback progress_cb)
        {
            if (!Directory.Exists(filepath))
                Directory.CreateDirectory(filepath);

            var data = matrix.data();
            UInt32 size_x = matrix.size_x();
            UInt32 size_y = matrix.size_y();

            int num_complete = 0;
            Parallel.For(0, height, kk =>
            {
                for (UInt32 slice = 0; slice < num_slices;  ++slice)
                {
                    string filename = filepath + "\\" + base_filename + "_" +
                                      kk.ToString() + "_" + slice.ToString() +
                                      ".bmp";
                    FileStream file = new FileStream(filename, FileMode.Create);
                    BinaryWriter writer = new BinaryWriter(file);

                    writer.Write('B');                      // Bitmap header
                    writer.Write('M');                      // Bitmap header
                    writer.Write(62 + length * width / 8);  // File size
                    writer.Write(0);                        // Reserved
                    writer.Write(62);                       // Image offset
                    writer.Write(40);                       // Header size
                    writer.Write(width);                    // Image width
                    writer.Write(length);                   // Image length
                    writer.Write((UInt16)1);                // Number of colour planes
                    writer.Write((UInt16)1);                // Bits per pixel
                    writer.Write(0);                        // Compression type
                    writer.Write(length * width / 8);       // Image size
                    writer.Write(0);                        // Horizontal resolution (unused)
                    writer.Write(0);                        // Vertical resolution (unused)
                    writer.Write(0);                        // Number of colours
                    writer.Write(0);                        // Number of important colours
                    writer.Write((UInt64)16777215);         // Palette info

                    for (UInt32 jj = slice * length; jj < (slice + 1) * length;
                         ++jj)
                    {
                        if (jj >= size_y) {
                            for (UInt32 ii = 0; ii < width / 32; ++ii)
                                writer.Write(0);
                            continue;
                        }

                        byte[] line = new byte[width];
                        for (UInt32 ii = 0; ii < width; ++ii)
                        {
                            if (ii < size_x)
                            {
                                for (UInt32 vv = 0; vv < z_scaling; ++vv)
                                    line[ii] |= (byte)data[(kk * z_scaling + vv) * size_y * size_x +
                                                           jj * size_x + ii];
                            }
                        }

                        for (UInt32 byt = 0; byt < width / 8; ++byt)
                        {
                            byte value = (byte)0;
                            for (UInt32 bit = 0; bit < 8; ++bit)
                                value |= (byte)(line[byt * 8 + bit] << (int)(7 - bit));
                            writer.Write(value);
                        }
                    }

                    writer.Close();
                    file.Close();
                }

                Interlocked.Increment(ref num_complete);
                progress_cb((double)num_complete / height, (int)Status.WRITE);
            });
        }
    }
}
