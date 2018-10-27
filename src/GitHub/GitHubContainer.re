type state =
  | Error(string)
  | Loading
  | Loaded(array(GitHub.notification));

type action =
  | MarkAllAsRead
  | MarkAsRead(string)
  | NotificationsError(string)
  | NotificationsFetch
  | NotificationsFetched(array(GitHub.notification));

let component = ReasonReact.reducerComponent("GitHubContainer");

let make = _children => {
  ...component,

  didMount: self => {
    self.send(NotificationsFetch);

    let intervalId =
      Js.Global.setInterval(
        () => self.send(NotificationsFetch),
        GitHub.Config.interval,
      );
    self.onUnmount(() => Js.Global.clearInterval(intervalId));
  },

  initialState: () => Loading,

  reducer: (action: action, state: state) =>
    switch (action) {
    | NotificationsError(err) => ReasonReact.Update(Error(err))
    | NotificationsFetch =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        (
          self =>
            Js.Promise.(
              GitHub.getNotifications()
              |> then_(notifications => {
                   DocumentTitle.updateTitleWithNotifications(
                     GitHub.Config.numberOfNotifications,
                     Trello.Config.numberOfNotifications,
                   );

                   Notify.sendGitHubNotification(notifications);

                   self.send(NotificationsFetched(notifications)) |> resolve;
                 })
              |> catch(_ => self.send(NotificationsError("err")) |> resolve)
              |> ignore
            )
        ),
      )
    | NotificationsFetched(notifications) =>
      ReasonReact.Update(Loaded(notifications))
    | MarkAllAsRead =>
      ReasonReact.UpdateWithSideEffects(
        switch (state) {
        | Loaded(_) =>
          DocumentTitle.updateTitleWithNotifications(
            "0",
            Trello.Config.numberOfNotifications,
          );

          GitHub.Config.setNumberOfNotifications([||]);
          Loaded([||]);
        | _ => Loading
        },
        (
          _self =>
            Js.Promise.(
              GitHub.markAllNotificationsAsRead()
              |> then_(_ => true |> resolve)
              |> ignore
            )
        ),
      )
    | MarkAsRead(id) =>
      ReasonReact.UpdateWithSideEffects(
        switch (state) {
        | Loaded(state) =>
          let filteredNotifications =
            state
            |> Js.Array.filter((item: GitHub.notification) => item.id !== id);

          DocumentTitle.updateTitleWithNotifications(
            Array.length(filteredNotifications) |> string_of_int,
            Trello.Config.numberOfNotifications,
          );

          GitHub.Config.setNumberOfNotifications(filteredNotifications);

          Loaded(filteredNotifications);
        | _ => Loading
        },
        (
          _self =>
            Js.Promise.(
              GitHub.markNotificationAsRead(id)
              |> then_(_ => true |> resolve)
              |> ignore
            )
        ),
      )
    },

  render: ({send, state}) =>
    <div className="w-100 w-50-l mr4 mb4">
      <Header
        color="b--dark-blue"
        hasItems={
          switch (state) {
          | Loaded(n) => Array.length(n) > 0
          | _ => false
          }
        }
        markAllAsRead={_ => send(MarkAllAsRead)}
        title="GitHub"
      />
      <Card>
        {
          GitHub.Config.hasConfig ?
            switch (state) {
            | Error(_) => <CardError />
            | Loading => <CardLoading />
            | Loaded(notifications) =>
              let groupedByDate = Utils.groupGitHubByDate(notifications);

              Belt.Array.length(notifications) > 0 ?
                Js.Dict.values(groupedByDate)
                ->Belt.Array.mapWithIndex((i, list) =>
                    <ListByDate
                      index=i
                      key={Js.Dict.keys(groupedByDate)[i]}
                      title={Js.Dict.keys(groupedByDate)[i]}
                      totalItems={
                        Array.length(Js.Dict.values(groupedByDate))
                      }>
                      {
                        list->Belt.Array.mapWithIndex(
                          (i, item: GitHub.notification) =>
                          <GitHubNotification
                            item
                            isLast={Array.length(list) - 1 === i}
                            key={item.id}
                            markAsRead=(_ => send(MarkAsRead(item.id)))
                          />
                        )
                        |> ReasonReact.array
                      }
                    </ListByDate>
                  )
                |> ReasonReact.array :
                <EmptyState />;
            } :
            <GitHubMissing />
        }
      </Card>
    </div>,
};
