/*
 * MainWindow.cpp
 *
 * (c) 2013 Sofian Audry -- info(@)sofianaudry(.)com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MainWindow.h"

MainWindow::MainWindow()
{
  sourceList = new QListView;
  sourceCanvas = new SourceGLCanvas;
  destinationCanvas = new DestinationGLCanvas(0, sourceCanvas);

  QObject::connect(sourceCanvas, SIGNAL(quadChanged()),
                   destinationCanvas, SLOT(updateCanvas()));

  QObject::connect(destinationCanvas, SIGNAL(quadSwitched()),
                   sourceCanvas, SLOT(updateCanvas()));

  sourceCanvas->setFocusPolicy(Qt::ClickFocus);
  destinationCanvas->setFocusPolicy(Qt::ClickFocus);

  QSplitter* mainSplitter = new QSplitter(Qt::Horizontal);

  QSplitter* canvasSplitter = new QSplitter(Qt::Horizontal);
  canvasSplitter->addWidget(sourceCanvas);
  canvasSplitter->addWidget(destinationCanvas);
  canvasSplitter->setMinimumWidth(DEFAULT_WIDTH * 2/3);

  mainSplitter->addWidget(sourceList);
  mainSplitter->addWidget(canvasSplitter);

  this->setWindowTitle(QObject::tr("Libremapping"));
  this->resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
  this->setCentralWidget(mainSplitter);

  Common::initializeLibremapper(sourceCanvas->width(), sourceCanvas->height());

  QStringListModel* sourcesModel = new QStringListModel;
  for (int i=0; i<Common::nImages(); i++)
  {
    std::tr1::shared_ptr<Image> img = std::tr1::static_pointer_cast<Image>(Common::mappings[i]->getPaint());
    Q_CHECK_PTR(img);

    sourcesModel->insertRow(sourcesModel->rowCount(), sourcesModel->index(1));
    sourcesModel->setData(sourcesModel->index(sourcesModel->rowCount()-1), img->getImagePath());
  }

  sourceList->setModel(sourcesModel);
}

MainWindow::~MainWindow() {
}
