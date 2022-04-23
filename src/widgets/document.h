#pragma once
#include <imgui.h>

// System includes
#include <ctype.h>          // toupper
#include <limits.h>         // INT_MIN, INT_MAX
#include <math.h>           // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>         // NULL, malloc, free, atoi
#include <stdint.h>         // intptr_t

struct DocOpener 
{
    struct MyDocument
    {
        const char* Name;       // Document title
        bool        Open;       // Set when open (we keep an array of all available documents to simplify demo code!)
        bool        OpenPrev;   // Copy of Open from last update.
        bool        Dirty;      // Set when the document has been modified
        bool        WantClose;  // Set when the document
        ImVec4      Color;      // An arbitrary variable associated to the document

        MyDocument(const char* name, bool open = true, const ImVec4& color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f))
        {
            Name = name;
            Open = OpenPrev = open;
            Dirty = false;
            WantClose = false;
            Color = color;
        }
        void DoOpen()       { Open = true; }
        void DoQueueClose() { WantClose = true; }
        void DoForceClose() { Open = false; Dirty = false; }
        void DoSave()       { Dirty = false; }

        // Display placeholder contents for the Document
        static void DisplayContents(MyDocument* doc)
        {
            ImGui::PushID(doc);
            ImGui::Text("Document \"%s\"", doc->Name);
            ImGui::PushStyleColor(ImGuiCol_Text, doc->Color);
            ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
            ImGui::PopStyleColor();
            if (ImGui::Button("Modify", ImVec2(100, 0)))
                doc->Dirty = true;
            ImGui::SameLine();
            if (ImGui::Button("Save", ImVec2(100, 0)))
                doc->DoSave();
            ImGui::ColorEdit3("color", &doc->Color.x);  // Useful to test drag and drop and hold-dragged-to-open-tab behavior.
            ImGui::PopID();
        }

        // Display context menu for the Document
        static void DisplayContextMenu(MyDocument* doc)
        {
            if (!ImGui::BeginPopupContextItem())
                return;

            char buf[256];
            sprintf(buf, "Save %s", doc->Name);
            if (ImGui::MenuItem(buf, "CTRL+S", false, doc->Open))
                doc->DoSave();
            if (ImGui::MenuItem("Close", "CTRL+W", false, doc->Open))
                doc->DoQueueClose();
            ImGui::EndPopup();
        }
    };

    struct ExampleAppDocuments
    {
        ImVector<MyDocument> Documents;

        ExampleAppDocuments()
        {
            Documents.push_back(MyDocument("Lettuce",             true,  ImVec4(0.4f, 0.8f, 0.4f, 1.0f)));
            Documents.push_back(MyDocument("Eggplant",            true,  ImVec4(0.8f, 0.5f, 1.0f, 1.0f)));
            Documents.push_back(MyDocument("Carrot",              true,  ImVec4(1.0f, 0.8f, 0.5f, 1.0f)));
            Documents.push_back(MyDocument("Tomato",              false, ImVec4(1.0f, 0.3f, 0.4f, 1.0f)));
            Documents.push_back(MyDocument("A Rather Long Title", false));
            Documents.push_back(MyDocument("Some Document",       false));
        }
    };

    // [Optional] Notify the system of Tabs/Windows closure that happened outside the regular tab interface.
    // If a tab has been closed programmatically (aka closed from another source such as the Checkbox() in the demo,
    // as opposed to clicking on the regular tab closing button) and stops being submitted, it will take a frame for
    // the tab bar to notice its absence. During this frame there will be a gap in the tab bar, and if the tab that has
    // disappeared was the selected one, the tab bar will report no selected tab during the frame. This will effectively
    // give the impression of a flicker for one frame.
    // We call SetTabItemClosed() to manually notify the Tab Bar or Docking system of removed tabs to avoid this glitch.
    // Note that this completely optional, and only affect tab bars with the ImGuiTabBarFlags_Reorderable flag.
    static void NotifyOfDocumentsClosedElsewhere(ExampleAppDocuments& app)
    {
        for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
        {
            MyDocument* doc = &app.Documents[doc_n];
            if (!doc->Open && doc->OpenPrev)
                ImGui::SetTabItemClosed(doc->Name);
            doc->OpenPrev = doc->Open;
        }
    }

    void ShowExampleAppDocuments()
    {
        static ExampleAppDocuments app;

        // Options
        static bool opt_reorderable = true;
        static ImGuiTabBarFlags opt_fitting_flags = ImGuiTabBarFlags_FittingPolicyDefault_;

        bool window_contents_visible = ImGui::Begin("Example: Documents");
        if (!window_contents_visible)
        {
            ImGui::End();
            return;
        }

        // Menu
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                int open_count = 0;
                for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
                    open_count += app.Documents[doc_n].Open ? 1 : 0;

                if (ImGui::BeginMenu("Open", open_count < app.Documents.Size))
                {
                    for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
                    {
                        MyDocument* doc = &app.Documents[doc_n];
                        if (!doc->Open)
                            if (ImGui::MenuItem(doc->Name))
                                doc->DoOpen();
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Close All Documents", NULL, false, open_count > 0))
                    for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
                        app.Documents[doc_n].DoQueueClose();
                if (ImGui::MenuItem("Exit", "Alt+F4")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // [Debug] List documents with one checkbox for each
        for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
        {
            MyDocument* doc = &app.Documents[doc_n];
            if (doc_n > 0)
                ImGui::SameLine();
            ImGui::PushID(doc);
            if (ImGui::Checkbox(doc->Name, &doc->Open))
                if (!doc->Open)
                    doc->DoForceClose();
            ImGui::PopID();
        }

        ImGui::Separator();

        // About the ImGuiWindowFlags_UnsavedDocument / ImGuiTabItemFlags_UnsavedDocument flags.
        // They have multiple effects:
        // - Display a dot next to the title.
        // - Tab is selected when clicking the X close button.
        // - Closure is not assumed (will wait for user to stop submitting the tab).
        //   Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
        //   We need to assume closure by default otherwise waiting for "lack of submission" on the next frame would leave an empty
        //   hole for one-frame, both in the tab-bar and in tab-contents when closing a tab/window.
        //   The rarely used SetTabItemClosed() function is a way to notify of programmatic closure to avoid the one-frame hole.

        // Submit Tab Bar and Tabs
        {
            ImGuiTabBarFlags tab_bar_flags = (opt_fitting_flags) | (opt_reorderable ? ImGuiTabBarFlags_Reorderable : 0);
            if (ImGui::BeginTabBar("##tabs", tab_bar_flags))
            {
                if (opt_reorderable)
                    NotifyOfDocumentsClosedElsewhere(app);

                // [DEBUG] Stress tests
                //if ((ImGui::GetFrameCount() % 30) == 0) docs[1].Open ^= 1;            // [DEBUG] Automatically show/hide a tab. Test various interactions e.g. dragging with this on.
                //if (ImGui::GetIO().KeyCtrl) ImGui::SetTabItemSelected(docs[1].Name);  // [DEBUG] Test SetTabItemSelected(), probably not very useful as-is anyway..

                // Submit Tabs
                for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
                {
                    MyDocument* doc = &app.Documents[doc_n];
                    if (!doc->Open)
                        continue;

                    ImGuiTabItemFlags tab_flags = (doc->Dirty ? ImGuiTabItemFlags_UnsavedDocument : 0);
                    bool visible = ImGui::BeginTabItem(doc->Name, &doc->Open, tab_flags);

                    // Cancel attempt to close when unsaved add to save queue so we can display a popup.
                    if (!doc->Open && doc->Dirty)
                    {
                        doc->Open = true;
                        doc->DoQueueClose();
                    }

                    MyDocument::DisplayContextMenu(doc);
                    if (visible)
                    {
                        MyDocument::DisplayContents(doc);
                        ImGui::EndTabItem();
                    }
                }

                ImGui::EndTabBar();
            }
        }

        // Update closing queue
        static ImVector<MyDocument*> close_queue;
        if (close_queue.empty())
        {
            // Close queue is locked once we started a popup
            for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
            {
                MyDocument* doc = &app.Documents[doc_n];
                if (doc->WantClose)
                {
                    doc->WantClose = false;
                    close_queue.push_back(doc);
                }
            }
        }

        // Display closing confirmation UI
        if (!close_queue.empty())
        {
            int close_queue_unsaved_documents = 0;
            for (int n = 0; n < close_queue.Size; n++)
                if (close_queue[n]->Dirty)
                    close_queue_unsaved_documents++;

            if (close_queue_unsaved_documents == 0)
            {
                // Close documents when all are unsaved
                for (int n = 0; n < close_queue.Size; n++)
                    close_queue[n]->DoForceClose();
                close_queue.clear();
            }
            else
            {
                if (!ImGui::IsPopupOpen("Save?"))
                    ImGui::OpenPopup("Save?");
                if (ImGui::BeginPopupModal("Save?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::Text("Save change to the following items?");
                    float item_height = ImGui::GetTextLineHeightWithSpacing();
                    if (ImGui::BeginChildFrame(ImGui::GetID("frame"), ImVec2(-FLT_MIN, 6.25f * item_height)))
                    {
                        for (int n = 0; n < close_queue.Size; n++)
                            if (close_queue[n]->Dirty)
                                ImGui::Text("%s", close_queue[n]->Name);
                        ImGui::EndChildFrame();
                    }

                    ImVec2 button_size(ImGui::GetFontSize() * 7.0f, 0.0f);
                    if (ImGui::Button("Yes", button_size))
                    {
                        for (int n = 0; n < close_queue.Size; n++)
                        {
                            if (close_queue[n]->Dirty)
                                close_queue[n]->DoSave();
                            close_queue[n]->DoForceClose();
                        }
                        close_queue.clear();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("No", button_size))
                    {
                        for (int n = 0; n < close_queue.Size; n++)
                            close_queue[n]->DoForceClose();
                        close_queue.clear();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel", button_size))
                    {
                        close_queue.clear();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
            }
        }

        ImGui::End();
    };
};
