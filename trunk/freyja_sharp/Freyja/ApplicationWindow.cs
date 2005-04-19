using System;
using Gtk;

public class ApplicationWindow : Window {
	
	public ApplicationWindow(string title, string icon) : base("Freyja#")
	{
		this.SetDefaultSize(400, 300);
		this.DeleteEvent += new DeleteEventHandler(onApplicationWindowDelete);

		/* Create test widgets */
		VBox vbox = new VBox();
	    HBox hbox = new HBox();
    	vbox.PackStart(hbox);

		ToggleButton togglebutton = new ToggleButton ("button1");
		togglebutton.Clicked += new EventHandler(clickedCallback);
		hbox.PackStart(togglebutton, false, false, 12);

		Button newbutton = new Button(Gtk.Stock.New);
		hbox.PackStart(newbutton, true, true, 12);
		
		Button openbutton = new Button(Gtk.Stock.Open);
		hbox.PackStart(openbutton, true, true, 12);
		
		Button savebutton = new Button(Gtk.Stock.Save);
		hbox.PackStart(savebutton, true, false, 12);
		
		Button saveasbutton = new Button(Gtk.Stock.SaveAs);
		hbox.PackStart(saveasbutton, true, false, 12);

   		this.Add(vbox);
		this.SetDefaultSize(200, 100);					
		this.ShowAll();
	}
	
	static void clickedCallback(object obj, EventArgs args)
	{
		/* Check Active Property */
		if (((ToggleButton)obj).Active)
			Console.WriteLine("ToggleButton clicked, I'm activating");
	}
	
	void onApplicationWindowDelete(object obj, DeleteEventArgs args)
	{
		Application.Quit();
	}
}