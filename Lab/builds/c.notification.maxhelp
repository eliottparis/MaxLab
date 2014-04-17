{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 6,
			"minor" : 1,
			"revision" : 6,
			"architecture" : "x64"
		}
,
		"rect" : [ 245.0, 274.0, 725.0, 385.0 ],
		"bgcolor" : [ 0.958029, 0.958029, 0.958029, 1.0 ],
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 0,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"boxes" : [ 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"frgb" : 0.0,
					"id" : "obj-20",
					"linecount" : 7,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 15.0, 181.0, 107.0, 100.0 ],
					"presentation_rect" : [ 53.0, 229.0, 0.0, 0.0 ],
					"text" : "Active metro then switch to another app to see notification popup and sound (depend on your preferences)."
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 1.0, 0.788235, 0.470588, 1.0 ],
					"border" : 0,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontname" : "Arial Bold",
					"fontsize" : 13.0,
					"hint" : "",
					"id" : "obj-21",
					"ignoreclick" : 1,
					"maxclass" : "textbutton",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "", "", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 15.0, 153.0, 20.0, 20.0 ],
					"presentation_rect" : [ 33.0, 229.0, 0.0, 0.0 ],
					"rounded" : 60.0,
					"text" : "4",
					"textcolor" : [ 0.34902, 0.34902, 0.34902, 1.0 ],
					"textovercolor" : [ 0.2, 0.2, 0.2, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"frgb" : 0.0,
					"id" : "obj-19",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 461.5, 87.5, 232.0, 33.0 ],
					"presentation_rect" : [ 435.0, 280.0, 0.0, 0.0 ],
					"text" : "Note : notifications only popup when Max App is in background."
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"frgb" : 0.0,
					"id" : "obj-18",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 492.0, 253.0, 219.0, 33.0 ],
					"presentation_rect" : [ 492.0, 257.0, 0.0, 0.0 ],
					"text" : "Does notification should play a sound when Max app is in background ?"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"frgb" : 0.0,
					"id" : "obj-15",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 324.0, 321.0, 275.0, 20.0 ],
					"presentation_rect" : [ 321.0, 321.0, 0.0, 0.0 ],
					"text" : "Open your notification center to see notifications"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 1.0, 0.788235, 0.470588, 1.0 ],
					"border" : 0,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontname" : "Arial Bold",
					"fontsize" : 13.0,
					"hint" : "",
					"id" : "obj-16",
					"ignoreclick" : 1,
					"maxclass" : "textbutton",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "", "", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 301.0, 321.0, 20.0, 20.0 ],
					"presentation_rect" : [ 301.0, 321.0, 0.0, 0.0 ],
					"rounded" : 60.0,
					"text" : "3",
					"textcolor" : [ 0.34902, 0.34902, 0.34902, 1.0 ],
					"textovercolor" : [ 0.2, 0.2, 0.2, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"frgb" : 0.0,
					"id" : "obj-5",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 166.0, 167.0, 150.0, 33.0 ],
					"presentation_rect" : [ 165.0, 156.0, 0.0, 0.0 ],
					"text" : "Bang to schedule a notification"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 1.0, 0.788235, 0.470588, 1.0 ],
					"border" : 0,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontname" : "Arial Bold",
					"fontsize" : 13.0,
					"hint" : "",
					"id" : "obj-3",
					"ignoreclick" : 1,
					"maxclass" : "textbutton",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "", "", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 144.0, 167.0, 20.0, 20.0 ],
					"presentation_rect" : [ 139.0, 166.0, 0.0, 0.0 ],
					"rounded" : 60.0,
					"text" : "2",
					"textcolor" : [ 0.34902, 0.34902, 0.34902, 1.0 ],
					"textovercolor" : [ 0.2, 0.2, 0.2, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "playsound",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-13",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 352.0, 253.0, 131.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "title",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-12",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 352.0, 153.0, 272.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "subtitle",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-11",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 352.0, 187.0, 272.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"frgb" : 0.0,
					"id" : "obj-7",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 51.0, 94.0, 150.0, 33.0 ],
					"text" : "Set notification title, subtitle and description"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 1.0, 0.788235, 0.470588, 1.0 ],
					"border" : 0,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontname" : "Arial Bold",
					"fontsize" : 13.0,
					"hint" : "",
					"id" : "obj-32",
					"ignoreclick" : 1,
					"maxclass" : "textbutton",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "", "", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 28.0, 94.0, 20.0, 20.0 ],
					"rounded" : 60.0,
					"text" : "1",
					"textcolor" : [ 0.34902, 0.34902, 0.34902, 1.0 ],
					"textovercolor" : [ 0.2, 0.2, 0.2, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-8",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 144.0, 190.0, 20.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-6",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 15.0, 289.0, 20.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-2",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 15.0, 321.0, 71.0, 20.0 ],
					"text" : "metro 4000"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-1",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 144.0, 293.0, 77.0, 20.0 ],
					"saved_object_attributes" : 					{
						"description" : [ "Notification", "description" ],
						"playsound" : 1,
						"subtitle" : [ "Notification", "subtitle" ],
						"title" : [ "Notification", "title" ]
					}
,
					"text" : "c.notification"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 11.595187,
					"frgb" : 0.0,
					"id" : "obj-4",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 7.0, 59.0, 431.0, 19.0 ],
					"text" : "c.notification is a MacOsx notification system accessor",
					"varname" : "autohelp_top_description[1]"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 3,
					"fontname" : "Arial",
					"fontsize" : 20.871338,
					"frgb" : 0.0,
					"id" : "obj-17",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 7.0, 7.0, 165.0, 30.0 ],
					"text" : "c.notification",
					"textcolor" : [ 0.963618, 0.963618, 0.963618, 1.0 ],
					"varname" : "autohelp_top_title"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.754705,
					"frgb" : 0.0,
					"id" : "obj-46",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 7.0, 35.0, 341.0, 21.0 ],
					"text" : "Send Notifications (MacOsx >= 10.8 Only)",
					"textcolor" : [ 0.963618, 0.963618, 0.963618, 1.0 ],
					"varname" : "autohelp_top_digest"
				}

			}
, 			{
				"box" : 				{
					"angle" : 85.0,
					"background" : 1,
					"border" : 1,
					"bordercolor" : [ 0.400462, 0.400462, 0.400462, 1.0 ],
					"grad1" : [ 0.552549, 0.552549, 0.552549, 1.0 ],
					"grad2" : [ 0.5, 0.5, 0.5, 1.0 ],
					"id" : "obj-23",
					"maxclass" : "panel",
					"mode" : 1,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 4.0, 4.0, 508.0, 52.0 ],
					"rounded" : 2,
					"varname" : "autohelp_top_panel"
				}

			}
, 			{
				"box" : 				{
					"angle" : 85.0,
					"bgcolor" : [ 0.926929, 0.926901, 0.926917, 1.0 ],
					"border" : 1,
					"bordercolor" : [ 0.400462, 0.400462, 0.400462, 1.0 ],
					"grad1" : [ 0.552549, 0.552549, 0.552549, 1.0 ],
					"grad2" : [ 0.5, 0.5, 0.5, 1.0 ],
					"id" : "obj-22",
					"maxclass" : "panel",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 15.0, 181.0, 107.0, 100.0 ],
					"presentation_rect" : [ 44.0, 118.0, 0.0, 0.0 ],
					"rounded" : 2,
					"varname" : "autohelp_top_panel[1]"
				}

			}
, 			{
				"box" : 				{
					"angle" : 85.0,
					"bgcolor" : [ 0.926929, 0.926901, 0.926917, 1.0 ],
					"border" : 1,
					"bordercolor" : [ 0.400462, 0.400462, 0.400462, 1.0 ],
					"grad1" : [ 0.552549, 0.552549, 0.552549, 1.0 ],
					"grad2" : [ 0.5, 0.5, 0.5, 1.0 ],
					"id" : "obj-24",
					"maxclass" : "panel",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 166.0, 167.0, 150.0, 33.0 ],
					"rounded" : 2,
					"varname" : "autohelp_top_panel[2]"
				}

			}
, 			{
				"box" : 				{
					"angle" : 85.0,
					"bgcolor" : [ 0.926929, 0.926901, 0.926917, 1.0 ],
					"border" : 1,
					"bordercolor" : [ 0.400462, 0.400462, 0.400462, 1.0 ],
					"grad1" : [ 0.552549, 0.552549, 0.552549, 1.0 ],
					"grad2" : [ 0.5, 0.5, 0.5, 1.0 ],
					"id" : "obj-25",
					"maxclass" : "panel",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 51.0, 94.0, 150.0, 33.0 ],
					"rounded" : 2,
					"varname" : "autohelp_top_panel[3]"
				}

			}
, 			{
				"box" : 				{
					"angle" : 85.0,
					"bgcolor" : [ 0.926929, 0.926901, 0.926917, 1.0 ],
					"border" : 2,
					"bordercolor" : [ 0.763956, 0.0, 0.032653, 0.75 ],
					"grad1" : [ 0.552549, 0.552549, 0.552549, 1.0 ],
					"grad2" : [ 0.5, 0.5, 0.5, 1.0 ],
					"id" : "obj-26",
					"maxclass" : "panel",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 461.5, 87.5, 232.0, 33.0 ],
					"rounded" : 2,
					"varname" : "autohelp_top_panel[4]"
				}

			}
, 			{
				"box" : 				{
					"angle" : 85.0,
					"bgcolor" : [ 0.926929, 0.926901, 0.926917, 1.0 ],
					"border" : 1,
					"bordercolor" : [ 0.400462, 0.400462, 0.400462, 1.0 ],
					"grad1" : [ 0.552549, 0.552549, 0.552549, 1.0 ],
					"grad2" : [ 0.5, 0.5, 0.5, 1.0 ],
					"id" : "obj-27",
					"maxclass" : "panel",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 324.0, 321.0, 275.0, 20.0 ],
					"rounded" : 2,
					"varname" : "autohelp_top_panel[5]"
				}

			}
, 			{
				"box" : 				{
					"attr" : "description",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-10",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 352.0, 221.0, 272.0, 20.0 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-10", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-11", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-12", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-13", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"midpoints" : [ 24.5, 350.0, 132.0, 350.0, 132.0, 283.0, 153.5, 283.0 ],
					"source" : [ "obj-2", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-2", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-6", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-8", 0 ]
				}

			}
 ],
		"dependency_cache" : [ 			{
				"name" : "c.notification.mxo",
				"type" : "iLaX"
			}
 ]
	}

}
