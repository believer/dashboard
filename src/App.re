type state = {
  displayGit: bool,
  displayTrello: bool,
};

type company =
  | GitHub
  | Trello;

type action =
  | Display(company)
  | Hide(company);

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,

  initialState: () => {displayGit: true, displayTrello: true},

  reducer: (action, state) =>
    switch (action) {
    | Display(GitHub) => ReasonReact.Update({...state, displayGit: true})
    | Display(Trello) => ReasonReact.Update({...state, displayTrello: true})
    | Hide(GitHub) => ReasonReact.Update({...state, displayGit: false})
    | Hide(Trello) => ReasonReact.Update({...state, displayTrello: false})
    },

  render: ({state, send}) =>
    <div className="flex h-100">
      <Navigation
        displayGit={state.displayGit}
        displayTrello={state.displayTrello}
        toggleGit={
          _ => send(state.displayGit ? Hide(GitHub) : Display(GitHub))
        }
        toggleTrello={
          _ => send(state.displayTrello ? Hide(Trello) : Display(Trello))
        }
      />
      {
        true ?
          <div className="w-100 flex flex-column flex-row-l pv4 ph5">
            {state.displayGit ? <GitHubContainer /> : ReasonReact.null}
            {state.displayTrello ? <TrelloContainer /> : ReasonReact.null}
          </div> :
          <div className="w-100 items-center flex justify-center">
            {"Add config to settings" |> Utils.str}
          </div>
      }
    </div>,
};
