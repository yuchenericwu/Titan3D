namespace Titan3D
{
    partial class ImportForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ImportForm));
            this.filenameTextBox = new System.Windows.Forms.TextBox();
            this.importButton = new System.Windows.Forms.Button();
            this.unitMmButton = new System.Windows.Forms.RadioButton();
            this.unitInchButton = new System.Windows.Forms.RadioButton();
            this.browseButton = new System.Windows.Forms.Button();
            this.importLabel = new System.Windows.Forms.Label();
            this.unitsLabel = new System.Windows.Forms.Label();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.progressBar = new System.Windows.Forms.ToolStripProgressBar();
            this.statusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.printButton = new System.Windows.Forms.Button();
            this.importBGWorker = new System.ComponentModel.BackgroundWorker();
            this.cancelButton = new System.Windows.Forms.Button();
            this.scalingLabel = new System.Windows.Forms.Label();
            this.scalingTextBox = new System.Windows.Forms.TextBox();
            this.writeBGWorker = new System.ComponentModel.BackgroundWorker();
            this.printBGWorker = new System.ComponentModel.BackgroundWorker();
            this.statusStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // filenameTextBox
            // 
            this.filenameTextBox.Location = new System.Drawing.Point(12, 32);
            this.filenameTextBox.Name = "filenameTextBox";
            this.filenameTextBox.Size = new System.Drawing.Size(324, 22);
            this.filenameTextBox.TabIndex = 0;
            // 
            // importButton
            // 
            this.importButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.importButton.Location = new System.Drawing.Point(342, 60);
            this.importButton.Name = "importButton";
            this.importButton.Size = new System.Drawing.Size(75, 33);
            this.importButton.TabIndex = 1;
            this.importButton.Text = "Import";
            this.importButton.UseVisualStyleBackColor = true;
            this.importButton.Click += new System.EventHandler(this.importButton_Click);
            // 
            // unitMmButton
            // 
            this.unitMmButton.Appearance = System.Windows.Forms.Appearance.Button;
            this.unitMmButton.CheckAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.unitMmButton.Checked = true;
            this.unitMmButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.unitMmButton.Location = new System.Drawing.Point(250, 60);
            this.unitMmButton.Name = "unitMmButton";
            this.unitMmButton.Size = new System.Drawing.Size(40, 25);
            this.unitMmButton.TabIndex = 2;
            this.unitMmButton.TabStop = true;
            this.unitMmButton.Text = "mm";
            this.unitMmButton.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.unitMmButton.UseVisualStyleBackColor = true;
            // 
            // unitInchButton
            // 
            this.unitInchButton.Appearance = System.Windows.Forms.Appearance.Button;
            this.unitInchButton.CheckAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.unitInchButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.unitInchButton.Location = new System.Drawing.Point(296, 60);
            this.unitInchButton.Name = "unitInchButton";
            this.unitInchButton.Size = new System.Drawing.Size(40, 25);
            this.unitInchButton.TabIndex = 3;
            this.unitInchButton.Text = "in";
            this.unitInchButton.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.unitInchButton.UseVisualStyleBackColor = true;
            // 
            // browseButton
            // 
            this.browseButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.browseButton.Location = new System.Drawing.Point(342, 21);
            this.browseButton.Name = "browseButton";
            this.browseButton.Size = new System.Drawing.Size(75, 33);
            this.browseButton.TabIndex = 4;
            this.browseButton.Text = "Browse";
            this.browseButton.UseVisualStyleBackColor = true;
            this.browseButton.Click += new System.EventHandler(this.browseButton_Click);
            // 
            // importLabel
            // 
            this.importLabel.AutoSize = true;
            this.importLabel.Location = new System.Drawing.Point(12, 12);
            this.importLabel.Name = "importLabel";
            this.importLabel.Size = new System.Drawing.Size(171, 17);
            this.importLabel.TabIndex = 5;
            this.importLabel.Text = "Select STL File To Import:";
            // 
            // unitsLabel
            // 
            this.unitsLabel.AutoSize = true;
            this.unitsLabel.Location = new System.Drawing.Point(200, 64);
            this.unitsLabel.Name = "unitsLabel";
            this.unitsLabel.Size = new System.Drawing.Size(44, 17);
            this.unitsLabel.TabIndex = 6;
            this.unitsLabel.Text = "Units:";
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.progressBar,
            this.statusLabel});
            this.statusStrip.Location = new System.Drawing.Point(0, 103);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(426, 25);
            this.statusStrip.TabIndex = 8;
            // 
            // progressBar
            // 
            this.progressBar.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(100, 19);
            this.progressBar.Step = 1;
            this.progressBar.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            // 
            // statusLabel
            // 
            this.statusLabel.Name = "statusLabel";
            this.statusLabel.Size = new System.Drawing.Size(50, 20);
            this.statusLabel.Text = "Ready";
            // 
            // printButton
            // 
            this.printButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.printButton.Location = new System.Drawing.Point(342, 60);
            this.printButton.Name = "printButton";
            this.printButton.Size = new System.Drawing.Size(75, 33);
            this.printButton.TabIndex = 9;
            this.printButton.Text = "Print";
            this.printButton.UseVisualStyleBackColor = true;
            this.printButton.Visible = false;
            this.printButton.Click += new System.EventHandler(this.printButton_Click);
            // 
            // importBGWorker
            // 
            this.importBGWorker.WorkerReportsProgress = true;
            this.importBGWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.importBGWorker_DoWork);
            this.importBGWorker.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.importBGWorker_ProgressChanged);
            this.importBGWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.importBGWorker_RunWorkerCompleted);
            // 
            // cancelButton
            // 
            this.cancelButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.cancelButton.Location = new System.Drawing.Point(342, 60);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 33);
            this.cancelButton.TabIndex = 12;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Visible = false;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // scalingLabel
            // 
            this.scalingLabel.AutoSize = true;
            this.scalingLabel.Location = new System.Drawing.Point(12, 64);
            this.scalingLabel.Name = "scalingLabel";
            this.scalingLabel.Size = new System.Drawing.Size(58, 17);
            this.scalingLabel.TabIndex = 13;
            this.scalingLabel.Text = "Scaling:";
            // 
            // scalingTextBox
            // 
            this.scalingTextBox.Location = new System.Drawing.Point(76, 61);
            this.scalingTextBox.MaxLength = 5;
            this.scalingTextBox.Name = "scalingTextBox";
            this.scalingTextBox.Size = new System.Drawing.Size(50, 22);
            this.scalingTextBox.TabIndex = 14;
            this.scalingTextBox.Text = "1.0";
            // 
            // writeBGWorker
            // 
            this.writeBGWorker.WorkerReportsProgress = true;
            this.writeBGWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.writeBGWorker_DoWork);
            this.writeBGWorker.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.writeBGWorker_ProgressChanged);
            this.writeBGWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.writeBGWorker_RunWorkerCompleted);
            // 
            // printBGWorker
            // 
            this.printBGWorker.WorkerReportsProgress = true;
            this.printBGWorker.WorkerSupportsCancellation = true;
            this.printBGWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.printBGWorker_DoWork);
            this.printBGWorker.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.printBGWorker_ProgressChanged);
            this.printBGWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.printBGWorker_RunWorkerCompleted);
            // 
            // ImportForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(426, 128);
            this.Controls.Add(this.scalingTextBox);
            this.Controls.Add(this.scalingLabel);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.unitsLabel);
            this.Controls.Add(this.importLabel);
            this.Controls.Add(this.browseButton);
            this.Controls.Add(this.unitInchButton);
            this.Controls.Add(this.unitMmButton);
            this.Controls.Add(this.filenameTextBox);
            this.Controls.Add(this.printButton);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.importButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ImportForm";
            this.Text = "Titan 3D Printer";
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox filenameTextBox;
        private System.Windows.Forms.Button importButton;
        private System.Windows.Forms.RadioButton unitMmButton;
        private System.Windows.Forms.RadioButton unitInchButton;
        private System.Windows.Forms.Button browseButton;
        private System.Windows.Forms.Label importLabel;
        private System.Windows.Forms.Label unitsLabel;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripStatusLabel statusLabel;
        private System.Windows.Forms.Button printButton;
        private System.ComponentModel.BackgroundWorker importBGWorker;
        private System.Windows.Forms.ToolStripProgressBar progressBar;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Label scalingLabel;
        private System.Windows.Forms.TextBox scalingTextBox;
        private System.ComponentModel.BackgroundWorker writeBGWorker;
        private System.ComponentModel.BackgroundWorker printBGWorker;
    }
}

