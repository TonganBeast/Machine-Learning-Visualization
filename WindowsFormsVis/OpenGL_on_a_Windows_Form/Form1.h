#pragma once

#include "OpenGL.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

std::string nameOfFile;
MD_Graph clear("test1.txt");
bool showAxes = true;

namespace OpenGL_on_a_Windows_Form {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace OpenGLForm;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			OpenGL = gcnew COpenGL(this, 1000, 400);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::ComponentModel::IContainer^  components;
	protected:
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button3;





	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		OpenGLForm::COpenGL ^ OpenGL;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(12, 521);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(285, 165);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Change File";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(303, 521);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(285, 165);
			this->button3->TabIndex = 2;
			this->button3->Text = L"Randomize";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1570, 698);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button1);
			this->Margin = System::Windows::Forms::Padding(4);
			this->MinimumSize = System::Drawing::Size(1100, 700);
			this->Name = L"Form1";
			this->Text = L"OpenGL on a Windows Form using Managed C++";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		UNREFERENCED_PARAMETER(sender);
		UNREFERENCED_PARAMETER(e);
		OpenGL->Render();
		OpenGL->SwapOpenGLBuffers();
	}

	std::string toStandardString(System::String^ str)
		{
			const char* location = (const char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str).ToPointer();
			std::string temp(location);
			/* Clean up the const char* location memory */
			System::Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr((void*)location));
			return temp;
		}

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
	{
		graph = clear;
		clear.getGraph().buildGraph();

		std::string grafPhile;
		OpenFileDialog^ ofd = gcnew OpenFileDialog;
		ofd->InitialDirectory = ""; // NOTE "c:\\" for future reference.
		ofd->Filter = "Text Files (*.csv, *.txt, *.text)|*.csv; *.txt; *.text";
		ofd->FilterIndex = 1;
		ofd->RestoreDirectory = true;

		if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			System::String^ path = ofd->FileName;

			grafPhile = toStandardString(path);
			nameOfFile = toStandardString(path);
		}
		
		MD_Graph gra(grafPhile);
		graph = gra;
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0, (graph.getGraph().getNumPlanes() * 300 + 100), 0.0, SCREEN_HEIGHT);

		gra.getGraph().buildGraph();
	}

	/*
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		showAxes = !showAxes;

		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0, (graph.getGraph().getNumPlanes() * 300 + 100), 0.0, SCREEN_HEIGHT);
		graph.getGraph().buildGraph(true, showAxes, false, true);
	}
	*/
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		graph = clear;
		clear.getGraph().buildGraph();

		MD_Graph graf(nameOfFile);
		graph = graf;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0, (graph.getGraph().getNumPlanes() * 300 + 100), 0.0, SCREEN_HEIGHT);

		graf.getGraph().buildGraph();
	}

	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
	}
	

};
}

