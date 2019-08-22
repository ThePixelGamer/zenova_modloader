using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.Threading;

using MahApps.Metro.Controls;
using System.IO;

namespace ZenovaWPF {
    public static class ExtensionMethods {
        private static Action Delegate = delegate () { };

        public static void Refresh(this UIElement element) {
            element.Dispatcher.Invoke(DispatcherPriority.Render, Delegate);
        }
    }

    public partial class MainWindow : MetroWindow {
        private StateChangeCallback test;
        public MainWindow() {
            InitializeComponent();
            test = new StateChangeCallback(this);

            FillModList(ref listBox);
        }

        public void CloseWindow() {

        }

        private void Tile_Click(object sender, RoutedEventArgs e) {
            //string oldText = PlayButton.Count;
            //PlayButton.Count = "Launching...";
            //PlayButton.Refresh();
            LauncherWrapper.LaunchMinecraft(true);
            //PlayButton.Count = oldText;
            //PlayButton.Refresh();
        }

        private void OpenMinecraftFolder(object sender, RoutedEventArgs e) {
            LauncherWrapper.OpenMinecraftFolder();
        }

        private void OpenModsFolder(object sender, RoutedEventArgs e) {
            LauncherWrapper.OpenModsFolder();
        }

        private void AddMod(object sender, RoutedEventArgs e) {

        }

        private void FillModList(ref ListBox tmp) {
            var dirs = Directory.GetFiles(@Environment.ExpandEnvironmentVariables("%AppData%") + "\\Zenova\\Mods", "*.dll").Select(System.IO.Path.GetFileName);
            foreach(string fileName in dirs)
                tmp.Items.Add(fileName);
        }
        public void updatePlayText(string text) {
            PlayButton.Count = text;
            PlayButton.Refresh();
        }
    }
}
