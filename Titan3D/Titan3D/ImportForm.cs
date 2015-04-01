using CppCliDll;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;

namespace Titan3D
{

    public delegate void UpdateProgressCallback(double progress, int status_enum);

    public enum Status
    {
        PARSE,
        SHELL,
        FILL,
        WRITE
    }

    public partial class ImportForm : Form
    {
        private const string remote_filepath = "Y:\\Data\\";
        private string filename;
        private string status;
        private bool remote_write = false;
        private CppCliPart part;
        private BitmapWriter writer;
        private ServerCom server;
        private readonly byte[] ip_address = { 10, 0, 1, 10 };
        private const int port = 13374;
        const double in_to_mm = 25.4;
        
        private Color gray = Color.FromArgb(51, 51, 51);
        private Color light_gray = Color.FromArgb(80, 80, 80);
        private Color blue = Color.FromArgb(69, 179, 231);

        private void update_progress(double progress, int status_enum)
        {
            switch ((Status)status_enum)
            {
                case Status.PARSE:
                    status = "Parsing file... ";
                    importBGWorker.ReportProgress((int)(progress * 100));
                    break;
                case Status.SHELL:
                    status = "Creating shell... ";
                    importBGWorker.ReportProgress((int)(progress * 100));
                    break;
                case Status.FILL:
                    status = "Filling part... ";
                    importBGWorker.ReportProgress((int)(progress * 100));
                    break;
                case Status.WRITE:
                    status = "Writing bitmaps... ";
                    writeBGWorker.ReportProgress((int)(progress * 100));
                    break;
            }
        }

        public ImportForm()
        {
            InitializeComponent();

            this.BackColor = gray;
            this.ForeColor = blue;

            statusStrip.BackColor = light_gray;
            statusStrip.ForeColor = blue;

            browseButton.BackColor = light_gray;
            browseButton.FlatAppearance.BorderSize = 0;
            importButton.BackColor = light_gray;
            importButton.FlatAppearance.BorderSize = 0;
            printButton.BackColor = light_gray;
            printButton.FlatAppearance.BorderSize = 0;

            unitMmButton.FlatAppearance.CheckedBackColor = light_gray;
            unitMmButton.FlatAppearance.BorderSize = 0;
            unitInchButton.FlatAppearance.CheckedBackColor = light_gray;
            unitInchButton.FlatAppearance.BorderSize = 0;
        }

        private void browseButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog fDialog = new OpenFileDialog();
            fDialog.Title = "Select STL File";
            fDialog.Filter = "STL Files|*.stl";
            fDialog.CheckFileExists = true;
            fDialog.CheckPathExists = true;

            if (fDialog.ShowDialog() == DialogResult.OK)
                filenameTextBox.Text = fDialog.FileName.ToString();
        }

        private void importButton_Click(object sender, EventArgs e)
        {
            double scaling;
            if (!Double.TryParse(scalingTextBox.Text, out scaling))
            {
                MessageBox.Show("Invalid scaling value.");
                return;
            }

            filenameTextBox.Enabled = false;
            browseButton.Enabled = false;
            importButton.Enabled = false;
            scalingTextBox.Enabled = false;
            unitMmButton.Enabled = false;
            unitInchButton.Enabled = false;
            importBGWorker.RunWorkerAsync(scaling);
        }

        private void importBGWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                string full_filename = filenameTextBox.Text;
                double scaling = (double)e.Argument;
                if (unitInchButton.Checked)
                    scaling *= in_to_mm;

                part = new CppCliPart(
                    full_filename, scaling,
                    (progress, status_enum) => update_progress(progress, status_enum));
                filename = Path.GetFileNameWithoutExtension(full_filename);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                Application.Restart();
            }
        }

        private void importBGWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            progressBar.Value = e.ProgressPercentage;
            statusLabel.Text = status + e.ProgressPercentage.ToString() + "%";
        }

        private void importBGWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            statusLabel.Text = "Importing... Complete";
            printButton.Visible = true;
            importButton.Visible = false;
        }

        private void printButton_Click(object sender, EventArgs e)
        {
            statusLabel.Text = "Connecting...";
            progressBar.Value = 0;
            printButton.Enabled = false;
            writeBGWorker.RunWorkerAsync();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            try
            {
                server.stop();
                statusLabel.Text = "Printing... Cancelled";
                printButton.Visible = true;
                printButton.Enabled = true;
                cancelButton.Visible = false;
            }
            catch
            {
                MessageBox.Show("Unable to connect to printer.");
            }
        }

        private void writeBGWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            UpdateProgressCallback progress_cb = new UpdateProgressCallback(update_progress);

            try
            {
                remote_write = true;
                writer = new BitmapWriter(remote_filepath + filename, filename, part.matrix());
                writer.Write(progress_cb);
            }
            catch
            {
                remote_write = false;
                try
                {
                    string desktop = 
                        Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory) + "\\";
                    writer = new BitmapWriter(desktop + filename, filename, part.matrix());
                    writer.Write(progress_cb);
                    MessageBox.Show("Unable to connect to printer. Output images were stored on the desktop.");
                }
                catch
                {
                    MessageBox.Show("Unable to connect to printer and unable to store images on the desktop.");
                }

                statusLabel.Text = "Connecting... Failed";
                printButton.Enabled = true;
            }
        }

        private void writeBGWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            progressBar.Value = e.ProgressPercentage;
            statusLabel.Text = status + e.ProgressPercentage.ToString() + "%";
        }

        private void writeBGWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (remote_write)
                print_part();
        }

        private void print_part()
        {
            try
            {
                statusLabel.Text = "Printing...";
                server = new ServerCom(ip_address, port);
                server.start(filename, writer.width, writer.num_slices, writer.height);
                cancelButton.Visible = true;
                printButton.Visible = false;
            }
            catch
            {
                MessageBox.Show("Unable to connect to printer.");
                statusLabel.Text = "Printing... Failed";
                printButton.Enabled = true;
                printButton.Visible = true;
                return;
            }

            Thread.Sleep(5000);
            printBGWorker.RunWorkerAsync();
        }

        private void printBGWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                int progress = 0;
                XmlReaderSettings xml_settings = new XmlReaderSettings();
                xml_settings.ConformanceLevel = ConformanceLevel.Fragment;

                while (progress < 100)
                {
                    XmlReader reader = XmlReader.Create(remote_filepath + "status.xml", xml_settings);
                    reader.ReadToFollowing("Current_Progress");
                    status = "Printing... ";
                    progress = reader.ReadElementContentAsInt();
                    printBGWorker.ReportProgress(progress);
                    reader.Close();
                    Thread.Sleep(500);
                }
            }
            catch
            {
                printBGWorker.CancelAsync();
            }
        }

        private void printBGWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            progressBar.Value = e.ProgressPercentage;
            statusLabel.Text = status + e.ProgressPercentage.ToString() + "%";
        }

        private void printBGWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Cancelled)
            {
                statusLabel.Text = "Printing... Status unknown";
                MessageBox.Show("Lost connection to printer.");
            }
            else
            {
                statusLabel.Text = "Printing... Complete";
                MessageBox.Show("Part printed successfully.");
            }
            Application.Restart();
        }
    }
}
